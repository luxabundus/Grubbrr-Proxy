#include "pch.h"
#include "ProxyModel.h"


#if defined(_DEBUG)
	#if defined(_WIN64)
	#define DEV_PLUGIN_DIR "..\\..\\Output\\Debug\\x64\\"
	#else
	#define DEV_PLUGIN_DIR "..\\..\\Output\\Debug\\Win32\\"
	#endif
#else
	#if defined(_WIN64)
	#define DEV_PLUGIN_DIR "..\\..\\Output\\Release\\x64\\"
	#else
	#define DEV_PLUGIN_DIR "..\\..\\Output\\Release\\Win32\\"
	#endif
#endif


ProxyModel::ProxyModel()
{
}


bool ProxyModel::init()
{
	m_pluginRootPath = AfxGetCurrentDirectory(true);
	if (AfxGetSessionId()) // running in a dev instance, i.e. under a user session.
	{
		m_pluginRootPath = AfxCanonicalizePath(m_pluginRootPath + DEV_PLUGIN_DIR);
		AfxSetCurrentDirectory(m_pluginRootPath);
	}

	return true;
}


void ProxyModel::exit()
{
	m_terminals.clear();
}


ProxyTerminal *ProxyModel::lookupTerminal(const String &terminalId)
{
	ProxyTerminalPtr pTerminal;

	auto res = m_terminals.find(terminalId);
	if (res != m_terminals.end())
	{
		pTerminal = res->second;
	}
	else
	{
		ProxyRegKey terminalKey;
		if (terminalKey.open(HKEY_LOCAL_MACHINE, ProxyRegKey::TERMINAL_ROOT_KEY + "\\" + terminalId) || (AfxGetLastError() == ERROR_FILE_NOT_FOUND))
		{
			pTerminal = new ProxyTerminal(terminalId);
			pTerminal->init(*this);
			m_terminals[terminalId] = pTerminal;
		}
	}

	return pTerminal;
}


ProxyPlugin *ProxyModel::loadPlugin(const String &pluginName, ProxyStringMap &params)
{
	ProxyPlugin *pPlugin = nullptr;

	ProxyRegKey settings;
	if (!settings.open(HKEY_LOCAL_MACHINE, String("%s\\%s", ProxyRegKey::PLUGIN_ROOT_KEY, pluginName)))
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "unknown plug-in: %s", pluginName);
	}
	settings.loadSettings(params);

	String pluginPath = m_pluginRootPath + settings.getStringValue("");

	try
	{
		pPlugin = ProxyPlugin::Load(pluginName, pluginPath, params);
	}
	catch (ProxyPlugin::Exception &x)
	{
		AfxLogError(x.code, "ProxyModel::loadPlugin@LoadPlugin(%s, %s)", pluginName, pluginPath);
		throw HttpException(HttpStatus::SERVER_ERROR, "error loading plug-in: %s", pluginName);
	}

	return pPlugin;
}