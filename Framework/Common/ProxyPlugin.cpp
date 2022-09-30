#include "pch.h"
#include "ProxyPlugin.h"


typedef ProxyPlugin * (*ProxyPluginFunctor)();


ProxyPlugin::ProxyPlugin() :
	m_hModule(NULL)
{
}


ProxyPlugin::~ProxyPlugin()
{
	if (m_hModule)
	{
		FreeLibrary(m_hModule);
	}
}


ProxyPlugin *ProxyPlugin::Load(const char *className, const char *filePath, ProxyStringMap &configParams)
{
	ProxyPlugin *pPlugin = nullptr;
	HMODULE hPlugin = ::LoadLibraryA(filePath);
	if (!hPlugin)
	{
		throw Exception(::GetLastError(), "LoadLibrary");
	}

	try
	{
		static const std::string FUNCTOR_ROOT = "Proxy_Create";
		std::string functorName = FUNCTOR_ROOT + className;
		ProxyPluginFunctor pfnCreatePlugin = (ProxyPluginFunctor)::GetProcAddress(hPlugin, functorName.data());
		if (!pfnCreatePlugin)
		{
			::FreeLibrary(hPlugin);
			throw Exception(::GetLastError(), "CreatePluginFunctor");
		}

		pPlugin = (*pfnCreatePlugin)();
		if (pPlugin)
		{
			pPlugin->init(configParams);
			pPlugin->m_hModule = hPlugin;
		}
		else if (::GetLastError() == ERROR_SUCCESS) // If really successful, we'd have a valid plugin.
		{
			throw Exception(ERROR_UNIDENTIFIED_ERROR, "CreatePlugin");
		}
		else
		{
			throw Exception(::GetLastError(), "CreatePlugin");
		}
	}
	catch (...)
	{
		if (pPlugin)
		{
			delete pPlugin;
		}

		::FreeLibrary(hPlugin);

		throw Exception(ERROR_UNIDENTIFIED_ERROR, "Load");;
	}

	return pPlugin;
}
