#pragma once


class ProxyTerminal : public RefCounter
{
public:
	ProxyTerminal(const String &terminalId);
	~ProxyTerminal();

	void init(ProxyModel &model);
	void exit();

	const String &getTerminalId();
	ProxyCardReaderPlugin *getCardReader();

private:
	String m_terminalId;
	ProxyCardReaderPlugin *m_pCardReader;

	ProxyPlugin *initPlugin(ProxyModel &model, const String &pluginType);
};


inline const String &ProxyTerminal::getTerminalId()
{
	return m_terminalId;
}

inline ProxyCardReaderPlugin *ProxyTerminal::getCardReader()
{
	return m_pCardReader;
}


using ProxyTerminalPtr = RefPointer<ProxyTerminal>;