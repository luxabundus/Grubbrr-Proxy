#pragma once


class ProxyApiServer : public ProxyServer
{
public:
	ProxyApiServer();

protected:
	virtual bool initServer(RegKey &settings);

	void initPluginRequest(ProxyCardReaderPlugin::TransactionRequest &pluginRequest, const Json &jsonRequest);
	void initJsonResponse(Json &jsonResponse, const ProxyCardReaderPlugin::TransactionResponse &pluginResponse);

	DECLARE_HTTP_MAP()

	void onPayment(HttpServerContext &context);
	void onRefund(HttpServerContext &context);
	void onStatus(HttpServerContext &context);

private:
};

