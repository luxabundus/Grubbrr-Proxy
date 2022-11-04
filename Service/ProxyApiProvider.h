#pragma once


class ProxyApiProvider : public ProxyProvider
{
public:
	ProxyApiProvider();

protected:
	virtual bool initServer(RegKey &settings);

	DECLARE_HTTP_MAP()

	void onCardStatus(HttpServerContext &context);
	void onCardPayment(HttpServerContext &context);
	void onCardRefund(HttpServerContext &context);
	void onCardVoid(HttpServerContext &context);
	void onCardSettlement(HttpServerContext &context);

private:
	class ApiAccessor : public AccessorBase
	{
	public:
		ApiAccessor(ProxyApiProvider *pServer, HttpServerContext &context);

	private:
		ProxyApiProvider *m_pServer;
	};

	ProxyTerminal &getTerminal(const String &terminalId);
	ProxyCardReaderPlugin &getCardReader(const String &terminalId);

	Json &validateRequestParam(Json &apiRequest, const char *fieldName);
	void validateSaleRequest(Json &apiRequest);
	void reformatTotalAmount(ProxyCardReaderPlugin::Transaction &transaction);

	Json execCardTransaction(
		Json &apiRequest,
		std::function<void(ProxyCardReaderPlugin&, ProxyCardReaderPlugin::Transaction&)> &&function);
};



inline ProxyApiProvider::ApiAccessor::ApiAccessor(ProxyApiProvider *pServer, HttpServerContext &context) :
	AccessorBase(pServer, context),
	m_pServer(pServer)
{
}

