#include "pch.h"
#include "ProxyApiServer.h"


ProxyApiServer::ProxyApiServer() :
	ProxyServer("API")
{
	registerCorsOrigin("");
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


Json ProxyApiServer::execCardTransaction(
	ApiAccessor &accessor,
	Json &apiRequest,
	std::function<ProxyStringMap(ProxyCardReaderPlugin&, ProxyStringMap&)> &&function)
{
	Json apiResponse;

	String terminalId = apiRequest["terminalId"];
	if (terminalId.isEmpty())
	{
		terminalId = "000";
	}

	ProxyCardReaderPlugin &cardReader = accessor.getCardReader(terminalId);

	try
	{
		ProxyStringMap cardRequest = JsonToProxy(apiRequest);
		ProxyStringMap cardResponse = function(cardReader, cardRequest);

		apiResponse = ProxyToJson(cardResponse);
	}
	catch (ProxyPlugin::Exception &x)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "(%d) %s", x.code, x.message);
	}

	return apiResponse;
}



ProxyApiServer::ApiAccessor::ApiAccessor(ProxyApiServer *pServer, HttpServerContext &context) :
	Accessor(pServer, context),
	m_pServer(pServer)
{
}

ProxyTerminal &ProxyApiServer::ApiAccessor::getTerminal(const String &terminalId)
{
	if (terminalId.isEmpty())
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "missing terminal-id");
	}

	ProxyTerminal *pTerminal = m_pServer->getModel().lookupTerminal(terminalId);
	if (!pTerminal)
	{
		throw HttpException(HttpStatus::BAD_REQUEST, "unknown terminal-id: %s", terminalId);
	}

	return *pTerminal;
}

ProxyCardReaderPlugin &ProxyApiServer::ApiAccessor::getCardReader(const String &terminalId)
{
	ProxyTerminal &terminal = getTerminal(terminalId);

	ProxyCardReaderPlugin *pCardReader = terminal.getCardReader();
	if (!pCardReader)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "undefined card-reader for terminal '%s'", terminalId);
	}

	return *pCardReader;
}
