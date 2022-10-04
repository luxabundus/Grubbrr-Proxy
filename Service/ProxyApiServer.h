#pragma once


class ProxyApiServer : public ProxyServer
{
	class Accessor
	{
	public:
		Accessor(ProxyApiServer *pServer, HttpServerContext &context);

		Json getApiRequest(bool throwIfUndefined = true);
		void setApiResponse(Json &apiResponse);

		ProxyTerminal &getTerminal(const String &terminalId);
		ProxyCardReaderPlugin &getCardReader(const String &terminalId);

	private:
		ProxyApiServer *m_pServer;
		HttpServerContext &m_context;
	};

public:
	ProxyApiServer();

protected:
	virtual bool initServer(RegKey &settings);

	Json execCardTransaction(
		Accessor &accessor,
		Json &apiRequest,
		std::function<ProxyStringMap(ProxyCardReaderPlugin&, ProxyStringMap&)> &&function);

	DECLARE_HTTP_MAP()

	void onPayment(HttpServerContext &context);
	void onRefund(HttpServerContext &context);
	void onSettle(HttpServerContext &context);
	void onStatus(HttpServerContext &context);
};
