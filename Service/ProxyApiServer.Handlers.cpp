#include "pch.h"
#include "ProxyApiServer.h"


BEGIN_HTTP_MAP(ProxyApiServer)
	ON_HTTP_METHOD("POST", "/payment", onPayment)
	ON_HTTP_METHOD("POST", "/refund", onRefund)
	ON_HTTP_METHOD("POST", "/settle", onSettle)
	ON_HTTP_METHOD("GET,POST", "/status", onStatus)
END_HTTP_MAP()


void ProxyApiServer::onPayment(HttpServerContext &context)
{
	Accessor accessor(this, context);

	Json apiRequest = accessor.getApiRequest();

	Json apiResponse = apiRequest;
	apiResponse["data"] = execCardTransaction(
		accessor,
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyStringMap &cardRequest) {
			return cardReader.sendPayment(cardRequest);
		}
	);

	accessor.setApiResponse(apiResponse);
}

void ProxyApiServer::onRefund(HttpServerContext &context)
{
	Accessor accessor(this, context);

	Json apiRequest = accessor.getApiRequest();

	Json apiResponse = apiRequest;
	apiResponse["data"] = execCardTransaction(
		accessor,
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyStringMap &cardRequest) {
			return cardReader.sendRefund(cardRequest);
		}
	);

	accessor.setApiResponse(apiResponse);
}

void ProxyApiServer::onSettle(HttpServerContext &context)
{
	Accessor accessor(this, context);

	Json apiRequest = accessor.getApiRequest();

	Json apiResponse = execCardTransaction(
		accessor,
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyStringMap &cardRequest) {
			return cardReader.settlePayments(cardRequest);
		}
	);

	accessor.setApiResponse(apiResponse);
}

void ProxyApiServer::onStatus(HttpServerContext &context)
{
	Accessor accessor(this, context);

	Json apiRequest = accessor.getApiRequest(false);
	if (apiRequest.isUndefined())
	{
		apiRequest["terminalId"] = context.request.getQueryParam("terminalId");
	}

	Json apiResponse = execCardTransaction(
		accessor,
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyStringMap &cardRequest) {
			return cardReader.queryStatus();
		}
	);

	accessor.setApiResponse(apiResponse);
}
