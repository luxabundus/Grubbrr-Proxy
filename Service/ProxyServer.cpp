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


