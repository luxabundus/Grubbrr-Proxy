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