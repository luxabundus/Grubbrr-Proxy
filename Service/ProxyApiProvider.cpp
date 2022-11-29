#include "pch.h"
#include "ProxyApiProvider.h"


BEGIN_HTTP_MAP(ProxyApiProvider)
	ON_HTTP_METHOD("GET,POST", "/card/status", onCardStatus)
	ON_HTTP_METHOD("POST", "/card/payment", onCardPayment)
	ON_HTTP_METHOD("POST", "/card/refund", onCardRefund)
	ON_HTTP_METHOD("POST", "/card/void", onCardVoid)
	ON_HTTP_METHOD("POST", "/card/settlement", onCardSettlement)
END_HTTP_MAP()


ProxyApiProvider::ProxyApiProvider() :
	ProxyProvider("API")
{
}


bool ProxyApiProvider::initServer(RegKey &settings)
{
	if (!ProxyProvider::initServer(settings))
	{
		return false;
	}

	String listener = settings.getStringValue("Listener");
	if (listener.isEmpty())
	{
		listener = "tcp:0.0.0.0:5432";
	}

	return startListener(listener);
}

Json &ProxyApiProvider::validateRequestParam(Json &apiRequest, const char *fieldName)
{
	if (apiRequest.hasProperty(fieldName))
	{
		return apiRequest[fieldName];
	}
	else
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "Missing %s", fieldName);
	}
}

void ProxyApiProvider::validateSaleRequest(Json &apiRequest)
{
	validateRequestParam(apiRequest, "terminalId");
	validateRequestParam(apiRequest, "orderId");
	validateRequestParam(apiRequest, "totalAmount");
}

void ProxyApiProvider::reformatTotalAmount(ProxyCardReaderPlugin::Transaction &transaction)
{
	double totalAmount = StringToDouble(transaction.requestData["totalAmount"]);

	// Apply (optional) divisor.
	if (transaction.requestData.contains("divisor"))
	{
		double divisor = StringToDouble(transaction.requestData["divisor"]);
		if (divisor)
		{
			totalAmount /= divisor;
		}
	}

	// Compute actual amount. (Note that callers specify amounts in hundreds.)
	totalAmount /= 100;

	// Update the amount sent to the plugin.
	transaction.requestData["totalAmount"] = (const char*)StringFromDouble(totalAmount, 2);
}

Json ProxyApiProvider::execCardTransaction(
	Json &apiRequest,
	std::function<void(ProxyCardReaderPlugin&, ProxyCardReaderPlugin::Transaction&)> &&function)
{
	Json apiResponse = apiRequest;	// Echo request params in the response.

	String terminalId = apiRequest["terminalId"];
	ProxyCardReaderPlugin &cardReader = getCardReader(terminalId);

	try
	{
		ProxyCardReaderPlugin::Transaction transaction;
		transaction.requestData = JsonToProxy(apiRequest);

		function(cardReader, transaction);

		Json &apiResult = apiResponse["result"] = Json::TYPE::Object;

		apiResult["status"] = (const char*)transaction.status;
		apiResult["statusCode"] = (const char*)transaction.statusCode;

		if (!transaction.statusMessage.empty())
		{
			apiResult["statusMessage"] = (const char*)transaction.statusMessage;
		}

		apiResult["data"] = ProxyToJson(transaction.resultData);
	}
	catch (ProxyPlugin::Exception &x)
	{
		throw HttpException(x.code, x.message);
	}

	return apiResponse;
}

ProxyTerminal &ProxyApiProvider::getTerminal(const String &terminalId)
{
	if (terminalId.isEmpty())
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "missing terminal-id");
	}

	ProxyTerminal *pTerminal = getModel().lookupTerminal(terminalId);
	if (!pTerminal)
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "unknown terminal-id: %s", terminalId);
	}

	return *pTerminal;
}

ProxyCardReaderPlugin &ProxyApiProvider::getCardReader(const String &terminalId)
{
	ProxyTerminal &terminal = getTerminal(terminalId);

	ProxyCardReaderPlugin *pCardReader = terminal.getCardReader();
	if (!pCardReader)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "undefined card-reader for terminal '%s'", terminalId);
	}

	return *pCardReader;
}


void ProxyApiProvider::onCardStatus(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);
	Json apiRequest = accessor.getRequest(false);

	validateRequestParam(apiRequest, "terminalId");

	Json apiResponse = execCardTransaction(
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyCardReaderPlugin::Transaction &transaction) {
			cardReader.queryStatus(transaction);
		}
	);

	context.response.setContent(apiResponse);
}

void ProxyApiProvider::onCardPayment(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);
	Json apiRequest = accessor.getRequest();

	validateSaleRequest(apiRequest);

	Json apiResponse = execCardTransaction(
		apiRequest,
		[this](ProxyCardReaderPlugin &cardReader, ProxyCardReaderPlugin::Transaction &transaction) mutable {
			reformatTotalAmount(transaction);
			cardReader.sendPayment(transaction);
		}
	);

	context.response.setContent(apiResponse);
}

void ProxyApiProvider::onCardRefund(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);
	Json apiRequest = accessor.getRequest();

	validateSaleRequest(apiRequest);
	validateRequestParam(apiRequest, "transactionId");

	Json apiResponse = execCardTransaction(
		apiRequest,
		[this](ProxyCardReaderPlugin &cardReader, ProxyCardReaderPlugin::Transaction &transaction) {
			reformatTotalAmount(transaction);
			cardReader.sendRefund(transaction);
		}
	);

	context.response.setContent(apiResponse);
}

void ProxyApiProvider::onCardVoid(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);
	Json apiRequest = accessor.getRequest();

	validateRequestParam(apiRequest, "terminalId");
	validateRequestParam(apiRequest, "transactionId");

	Json apiResponse = execCardTransaction(
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyCardReaderPlugin::Transaction &transaction) {
			cardReader.sendVoid(transaction);
		}
	);

	context.response.setContent(apiResponse);
}

void ProxyApiProvider::onCardSettlement(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);
	Json apiRequest = accessor.getRequest();

	validateRequestParam(apiRequest, "terminalId");

	Json apiResponse = execCardTransaction(
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyCardReaderPlugin::Transaction &transaction) {
			cardReader.sendSettlement(transaction);
		}
	);

	context.response.setContent(apiResponse);
}
