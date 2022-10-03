#include "pch.h"
#include "ProxyApiServer.h"


BEGIN_HTTP_MAP(ProxyApiServer)
	ON_HTTP_METHOD("POST", "/payment", onPayment)
	ON_HTTP_METHOD("POST", "/refund", onRefund)
	ON_HTTP_METHOD("GET", "/status", onStatus)
END_HTTP_MAP()


void ProxyApiServer::onPayment(HttpServerContext &context)
{
	String user, password;
	if (!context.request.getBasicAuth(user, password)
		|| (user != "Grubbrr.Proxy.Api.Dev")
		|| (password != "8TG$t37p!"))
	{
		context.response.challengeBasicAuth("Grubbrr.Dev");
		return;
	}

	Json jsonRequest, jsonResponse;

	if (!context.request.getContent(jsonRequest))
	{
		throw HttpException(HttpStatus::BAD_REQUEST);
	}

	// Lookup terminal configuration.
	String terminalId = jsonRequest["terminalId"];
	if (terminalId.isEmpty())
	{
		terminalId = "000";
	}

	ProxyTerminal *pTerminal = getModel().lookupTerminal(terminalId);
	if (!pTerminal)
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "unknown terminal-id: %s", terminalId);
	}

	ProxyCardReaderPlugin *pCardReader = pTerminal->getCardReader();
	if (!pCardReader)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "undefined card-reader");
	}

	ProxyCardReaderPlugin::TransactionRequest pluginRequest;
	ProxyCardReaderPlugin::TransactionResponse pluginResponse;

	initPluginRequest(pluginRequest, jsonRequest);

	try
	{
		pCardReader->sendPayment(pluginRequest, pluginResponse);
	}
	catch (ProxyPlugin::Exception &x)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "(%d) %s", x.code, x.message);
	}

	initJsonResponse(jsonResponse, pluginResponse);
	jsonResponse["echo"] = jsonRequest;

	context.response.setContent(jsonResponse);
}


void ProxyApiServer::onRefund(HttpServerContext &context)
{
	String user, password;
	if (!context.request.getBasicAuth(user, password)
		|| (user != "Grubbrr.Proxy.Api.Dev")
		|| (password != "8TG$t37p!"))
	{
		context.response.challengeBasicAuth("Grubbrr.Dev");
		return;
	}

	Json jsonRequest, jsonResponse;

	if (!context.request.getContent(jsonRequest))
	{
		throw HttpException(HttpStatus::BAD_REQUEST);
	}

	String terminalId = jsonRequest["terminalId"];
	if (terminalId.isEmpty())
	{
		terminalId = "000";
	}

	ProxyTerminal *pTerminal = getModel().lookupTerminal(terminalId);
	if (!pTerminal)
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "unknown terminal-id: %s", terminalId);
	}

	ProxyCardReaderPlugin *pCardReader = pTerminal->getCardReader();
	if (!pCardReader)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "undefined card-reader");
	}

	ProxyCardReaderPlugin::TransactionRequest pluginRequest;
	ProxyCardReaderPlugin::TransactionResponse pluginResponse;

	initPluginRequest(pluginRequest, jsonRequest);

	try
	{
		pCardReader->sendRefund(pluginRequest, pluginResponse);
	}
	catch (ProxyPlugin::Exception &x)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "(%d) %s", x.code, x.message);
	}

	initJsonResponse(jsonResponse, pluginResponse);
	jsonResponse["echo"] = jsonRequest;

	context.response.setContent(jsonResponse);
}


void ProxyApiServer::onStatus(HttpServerContext &context)
{
	context.response.setContent(String("Status"));
}
