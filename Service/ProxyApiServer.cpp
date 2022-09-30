#include "pch.h"
#include "ProxyApiServer.h"


ProxyApiServer::ProxyApiServer() :
	ProxyServer("API")
{
}


bool ProxyApiServer::initServer(RegKey &settings)
{
	if (!ProxyServer::initServer(settings))
	{
		return false;
	}

	String listener = settings.getStringValue("Listener");
	if (listener.isEmpty())
	{
		listener = "tcp:localhost:5432";
	}
	return startListener(listener);
}


void ProxyApiServer::initPluginRequest(ProxyCardReaderPlugin::TransactionRequest &pluginRequest, const Json &jsonRequest)
{
	pluginRequest.orderId = (const char*)jsonRequest["orderId"];
	if (pluginRequest.orderId.empty())
	{
		throw HttpException(HttpStatus::SERVER_ERROR, String("missing order-id"));
	}

	if (jsonRequest.hasProperty("subTotal"))
	{
		pluginRequest.subTotal = jsonRequest["subTotal"];
	}
	else
	{
		throw HttpException(HttpStatus::SERVER_ERROR, String("missing sub-total"));
	}

	if (jsonRequest.hasProperty("taxTotal"))
	{
		pluginRequest.taxTotal = jsonRequest["taxTotal"];
	}
	else
	{
		throw HttpException(HttpStatus::SERVER_ERROR, String("missing tax-total"));
	}

	if (jsonRequest.hasProperty("settle"))
	{
		pluginRequest.settle = jsonRequest["settle"];
	}
}

void ProxyApiServer::initJsonResponse(Json &jsonResponse, const ProxyCardReaderPlugin::TransactionResponse &pluginResponse)
{
	jsonResponse["transactionId"] = pluginResponse.transactionId.data();
	jsonResponse["status"] = pluginResponse.status.data();
	jsonResponse["statusCode"] = pluginResponse.statusCode.data();
	jsonResponse["statusMessage"] = pluginResponse.statusMessage.data();
	jsonResponse["cardScheme"] = pluginResponse.cardScheme.data();
}
