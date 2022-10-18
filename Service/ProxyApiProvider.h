#pragma once


class ProxyApiProvider : public ProxyProvider
{
public:
	ProxyApiProvider();

protected:
	virtual bool initServer(RegKey &settings);

	DECLARE_HTTP_MAP()

	void onPayment(HttpServerContext &context);
	void onRefund(HttpServerContext &context);
	void onSettle(HttpServerContext &context);
	void onStatus(HttpServerContext &context);

private:
	class ApiAccessor : public Accessor
	{
	public:
		ApiAccessor(ProxyApiProvider *pServer, HttpServerContext &context);

		ProxyTerminal &getTerminal(const String &terminalId);
		ProxyCardReaderPlugin &getCardReader(const String &terminalId);

	private:
		ProxyApiProvider *m_pServer;
	};

	Json execCardTransaction(
		ApiAccessor &accessor,
		Json &apiRequest,
		std::function<ProxyStringMap(ProxyCardReaderPlugin&, ProxyStringMap&)> &&function);
};
