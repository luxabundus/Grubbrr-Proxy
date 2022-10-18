#include "pch.h"
#include "ProxyApiProvider.h"


BEGIN_HTTP_MAP(ProxyApiProvider)
	ON_HTTP_METHOD("POST", "/payment", onPayment)
	ON_HTTP_METHOD("POST", "/refund", onRefund)
	ON_HTTP_METHOD("POST", "/settle", onSettle)
	ON_HTTP_METHOD("GET,POST", "/status", onStatus)
END_HTTP_MAP()


void ProxyApiProvider::onPayment(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);

	Json apiRequest = accessor.getRequest();

	Json apiResponse = apiRequest;	// echo request params
	apiResponse["data"] = execCardTransaction(
		accessor,
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyStringMap &cardRequest) {
			return cardReader.sendPayment(cardRequest);
		}
	);

	accessor.setResponse(apiResponse);
}

void ProxyApiProvider::onRefund(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);

	Json apiRequest = accessor.getRequest();

	Json apiResponse = apiRequest;	// echo request params
	apiResponse["data"] = execCardTransaction(
		accessor,
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyStringMap &cardRequest) {
			return cardReader.sendRefund(cardRequest);
		}
	);

	accessor.setResponse(apiResponse);
}

void ProxyApiProvider::onSettle(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);

	Json apiRequest = accessor.getRequest();

	Json apiResponse = execCardTransaction(
		accessor,
		apiRequest,
		[](ProxyCardReaderPlugin &cardReader, ProxyStringMap &cardRequest) {
			return cardReader.settlePayments(cardRequest);
		}
	);

	accessor.setResponse(apiResponse);
}

void ProxyApiProvider::onStatus(HttpServerContext &context)
{
	ApiAccessor accessor(this, context);

	Json apiRequest = accessor.getRequest(false);
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

	accessor.setResponse(apiResponse);
}
