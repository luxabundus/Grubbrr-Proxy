#include "pch.h"
#include "ProxyApiProvider.h"


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

	enableCors(86400);
	enableCorsCredentials();

	String listener = settings.getStringValue("Listener");
	if (listener.isEmpty())
	{
		listener = "tcp:0.0.0.0:5432";
	}

	return startListener(listener);
}


void ProxyApiProvider::validateRequestParam(const Json &apiRequest, const char *fieldName)
{
	if (!apiRequest.hasProperty(fieldName))
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "Missing %s", fieldName);
	}
}

void ProxyApiProvider::validateSaleRequest(const Json &apiRequest)
{
	validateRequestParam(apiRequest, "terminalId");
	validateRequestParam(apiRequest, "orderId");
	validateRequestParam(apiRequest, "totalAmount");
}


Json ProxyApiProvider::execCardTransaction(
	Json &apiRequest,
	std::function<void(ProxyCardReaderPlugin&, ProxyCardReaderPlugin::Transaction&)> &&function)
{
	Json apiResponse = apiRequest;	// Start with echo of request params.

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
