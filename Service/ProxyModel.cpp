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
	if (AfxGetSessionId()) // running a dev instance, i.e. under a user session.
	{
		m_pluginRootPath = AfxCanonicalizePath(m_pluginRootPath + DEV_PLUGIN_DIR);
	}


	/*
	* Plugins
	*/
/*
	RegKey pluginRootKey;
	if (!pluginRootKey.open(modelSettings, "Plugins"))
	{
		AfxLogLastError("ProxyModel::init@OpenPluginSettings");
		return false;
	}

	String pluginRootPath = AfxGetCurrentDirectory(true);
	if (AfxGetSessionId()) // running a dev instance, i.e. under a user session.
	{
		pluginRootPath = AfxCanonicalizePath(pluginRootPath + DEV_PLUGIN_DIR);
	}

	pluginRootKey.forEachKey(
		[this, &pluginRootPath, &pluginRootKey](const char *name) mutable
		{
			String pluginName = name;

			RegKey pluginSettings;
			if (pluginSettings.open(pluginRootKey, pluginName))
			{
				String pluginPath = pluginRootPath + pluginSettings.getStringValue("");

				ProxyStringMap params;
				pluginSettings.forEachStringValue(
					[this, &params](const char *name, const char *value) mutable
					{
						if (*name)
						{
							params["name"] = value;
						}
						return true;
					}
				);

				try
				{
					AfxLogInfo("loading '%s' from '%s'", pluginName, pluginPath);
					ProxyPlugin *pPlugin = ProxyPlugin::Load(pluginName, pluginPath, params);
					m_plugins[pluginName] = pPlugin;
					AfxLogInfo("loaded '%s'", pluginName);
				}
				catch (ProxyPlugin::Exception &x)
				{
					AfxLogError(x.code, "ProxyModel::init@LoadPlugin(%s) %s:", pluginPath, x.message);
				}
				catch (...)
				{
					AfxLogError("ProxyModel::init@LoadPlugin(%s) - unepxected error", pluginPath);
				}
			}
			else
			{
				AfxLogError("ProxyModel::init@OpenPluginKey(%s)", pluginName);
			}

			return true;
		}
	);
*/
	return true;
}


void ProxyModel::exit()
{
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
		pTerminal = new ProxyTerminal(terminalId);
		pTerminal->init(*this);
		m_terminals[terminalId] = pTerminal;
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
	catch (ProxyPlugin::Exception &)
	{
		AfxLogError("ProxyModel::loadPlugin@LoadPlugin(%s, %s)", pluginName, pluginPath);
		throw;
	}

	return pPlugin;
}