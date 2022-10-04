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


Json ProxyApiServer::execCardTransaction(
	Accessor &accessor,
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



ProxyApiServer::Accessor::Accessor(ProxyApiServer *pServer, HttpServerContext &context) :
	m_pServer(pServer),
	m_context(context)
{
	String user, password;
	if (!m_context.request.getBasicAuth(user, password)
		|| (user != "Grubbrr.Proxy.Api.Dev")
		|| (password != "8TG$t37p!"))
	{
		m_context.response.throwBasicAuth("Grubbrr.Proxy.Dev");
	}
}

Json ProxyApiServer::Accessor::getApiRequest(bool throwIfUndefined)
{
	Json apiRequest;
	if ((!m_context.request.getContent(apiRequest) || apiRequest.isUndefined())
		&& throwIfUndefined)
	{
		throw HttpException(HttpStatus::BAD_REQUEST);
	}
	return apiRequest;
}

void ProxyApiServer::Accessor::setApiResponse(Json &apiResponse)
{
	m_context.response.setContent(apiResponse);
}

ProxyTerminal &ProxyApiServer::Accessor::getTerminal(const String &terminalId)
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

ProxyCardReaderPlugin &ProxyApiServer::Accessor::getCardReader(const String &terminalId)
{
	ProxyTerminal &terminal = getTerminal(terminalId);

	ProxyCardReaderPlugin *pCardReader = terminal.getCardReader();
	if (!pCardReader)
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "undefined card-reader");
	}

	return *pCardReader;
}
