#include "pch.h"
#include "ProxyServer.h"


ProxyServer::ProxyServer(const String &name) :
	m_name(name),
	m_pModel(nullptr)
{
}


bool ProxyServer::init(ProxyModel &model)
{
	m_pModel = &model;

	RegKey settings;
	if (!settings.open(HKEY_LOCAL_MACHINE, String("%s\\%s", ProxyRegKey::ROOT_KEY, m_name)))
	{
		AfxLogLastError("ProxyServer::init@OpenSettingsKey(%s)", m_name);
		return false;
	}

	return initServer(settings);
}


bool ProxyServer::initServer(RegKey &settings)
{
	initHttpHandlers();

	if (!settings.getValue("AuthUser", m_authUser) && (AfxGetLastError() != ERROR_FILE_NOT_FOUND))
	{
		AfxLogLastError("ProxyServer::initServer@LoadAuthUser");
		return false;
	}
	if (!settings.getValue("AuthPassword", m_authPassword) && (AfxGetLastError() != ERROR_FILE_NOT_FOUND))
	{
		AfxLogLastError("ProxyServer::initServer@LoadAuthPassword");
		return false;
	}

	return true;
}


ProxyStringMap ProxyServer::JsonToProxy(const Json &json)
{
	ProxyStringMap proxy;
	for (auto it : json.byObject())
	{
		proxy[(const char*)it.first] = (const char*)it.second;
	}
	return proxy;
}

Json ProxyServer::ProxyToJson(const ProxyStringMap &proxy)
{
	Json json;
	for (auto it : proxy)
	{
		json[(const char*)it.first] = (const char*)it.second;
	}
	return json;
}


ProxyServer::Accessor::Accessor(ProxyServer *pServer, HttpServerContext &context) :
	m_context(context)
{
	String authUser, authPassword;
	if (!pServer->m_authUser.isEmpty())
	{
		String reqUser, reqPassword;
		if (!m_context.request.getBasicAuth(reqUser, reqPassword)
			|| (pServer->m_authUser != reqUser)
			|| (pServer->m_authPassword != reqPassword))
		{
			m_context.response.throwBasicAuth("Grubbrr.Proxy.Dev");
		}
	}
}

Json ProxyServer::Accessor::getRequest(bool throwIfUndefined)
{
	Json apiRequest;
	if ((!m_context.request.getContent(apiRequest) || apiRequest.isUndefined())
		&& throwIfUndefined)
	{
		throw HttpException(HttpStatus::BAD_REQUEST);
	}
	return apiRequest;
}

void ProxyServer::Accessor::setResponse(Json &apiResponse)
{
	m_context.response.setContent(apiResponse);
}
