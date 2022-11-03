#include "pch.h"
#include "ProxyProvider.h"


ProxyProvider::ProxyProvider(const String &name) :
	m_name(name),
	m_pModel(nullptr)
{
}


bool ProxyProvider::init(ProxyModel &model)
{
	m_pModel = &model;

	RegKey settings;
	if (!settings.open(HKEY_LOCAL_MACHINE, String("%s\\%s", ProxyRegKey::ROOT_KEY, m_name)))
	{
		AfxLogLastError("ProxyProvider::init@OpenSettingsKey(%s)", m_name);
		return false;
	}

	return initServer(settings);
}


bool ProxyProvider::initServer(RegKey &settings)
{
	initHttpHandlers();

	if (!settings.getValue("AuthUser", m_authUser) && (AfxGetLastError() != ERROR_FILE_NOT_FOUND))
	{
		AfxLogLastError("ProxyProvider::initServer@LoadAuthUser");
		return false;
	}
	if (!settings.getValue("AuthPassword", m_authPassword) && (AfxGetLastError() != ERROR_FILE_NOT_FOUND))
	{
		AfxLogLastError("ProxyProvider::initServer@LoadAuthPassword");
		return false;
	}

	return true;
}


ProxyStringMap ProxyProvider::JsonToProxy(const Json &json)
{
	ProxyStringMap proxy;
	for (auto it : json.byObject())
	{
		proxy[(const char*)it.first] = (const char*)it.second;
	}
	return proxy;
}

Json ProxyProvider::ProxyToJson(const ProxyStringMap &proxy)
{
	Json json;
	for (auto it : proxy)
	{
		json[(const char*)it.first] = (const char*)it.second;
	}
	return json;
}


ProxyProvider::AccessorBase::AccessorBase(ProxyProvider *pProvider, HttpServerContext &context) :
	m_context(context)
{
	String authUser, authPassword;
	if (!pProvider->m_authUser.isEmpty())
	{
		String reqUser, reqPassword;
		if (!m_context.request.getBasicAuth(reqUser, reqPassword)
			|| (pProvider->m_authUser != reqUser)
			|| (pProvider->m_authPassword != reqPassword))
		{
			m_context.response.throwBasicAuth("Grubbrr.Proxy.Dev");
		}
	}
}

Json ProxyProvider::AccessorBase::getRequest(bool throwIfUndefined)
{
	Json apiRequest;
	if ((!m_context.request.getContent(apiRequest) || apiRequest.isUndefined())
		&& throwIfUndefined)
	{
		throw HttpException(HttpStatus::BAD_REQUEST);
	}
	return apiRequest;
}

void ProxyProvider::AccessorBase::setResponse(Json &apiResponse)
{
	m_context.response.setContent(apiResponse);
}
