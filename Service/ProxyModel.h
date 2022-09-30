#pragma once


class ProxyModel
{
public:
	ProxyModel();

	bool init();
	void exit();

	ProxyTerminal *lookupTerminal(const String &terminalId);

	ProxyPlugin *loadPlugin(const String &pluginName, ProxyStringMap &params);

private:
	String m_pluginRootPath;
	std::map<StringKey, ProxyTerminalPtr> m_terminals;
};

