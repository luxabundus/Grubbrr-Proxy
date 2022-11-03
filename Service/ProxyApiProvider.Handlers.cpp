#include "pch.h"
#include "ProxyApiProvider.h"


BEGIN_HTTP_MAP(ProxyApiProvider)
	ON_HTTP_METHOD("GET,POST", "/card/status", onCardStatus)
	ON_HTTP_METHOD("POST", "/card/payment", onCardPayment)
	ON_HTTP_METHOD("POST", "/card/refund", onCardRefund)
	ON_HTTP_METHOD("POST", "/card/void", onCardVoid)
	ON_HTTP_METHOD("POST", "/card/settlement", onCardSettlement)
END_HTTP_MAP()


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
		[](ProxyCardReaderPlugin &cardReader, ProxyCardReaderPlugin::Transaction &transaction) {
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
		[](ProxyCardReaderPlugin &cardReader, ProxyCardReaderPlugin::Transaction &transaction) {
			cardReader.sendRefund(transaction);
		}
	);

	context.response.setContent(apiResponse);
}


void ProxyApiProvider::onCardVoid(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);
	Json apiRequest = accessor.getRequest();

	validateSaleRequest(apiRequest);
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
			cardReader.settlePayments(transaction);
		}
	);

	context.response.setContent(apiResponse);
}
