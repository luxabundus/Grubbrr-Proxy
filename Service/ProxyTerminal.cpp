#include "pch.h"
#include "ProxyTerminal.h"


ProxyTerminal::ProxyTerminal(const String &terminalId) :
	m_terminalId(terminalId),
	m_pCardReader(nullptr)
{
}

ProxyTerminal::~ProxyTerminal()
{
	exit();
}


void ProxyTerminal::init(ProxyModel &model)
{
	// Init card-reader.
	m_pCardReader = dynamic_cast<ProxyCardReaderPlugin*>(initPlugin(model, "CardReader"));
}


void ProxyTerminal::exit()
{
	if (m_pCardReader)
	{
		ProxyPlugin::Unload(m_pCardReader);
	}
}

ProxyPlugin *ProxyTerminal::initPlugin(ProxyModel &model, const String &pluginType)
{
	ProxyRegKey settings;
	if (!settings.open(HKEY_LOCAL_MACHINE, String("%s\\%s\\%s", ProxyRegKey::TERMINAL_ROOT_KEY, m_terminalId, pluginType)))
	{
		throw HttpException(HttpStatus::SERVER_ERROR, "undefined plug-in type: %s", pluginType);
	}

	ProxyStringMap params;
	settings.loadSettings(params);

	String pluginName = settings.getStringValue(nullptr);
	ProxyPlugin *pPlugin = model.loadPlugin(pluginName, params);

	return pPlugin;
}
