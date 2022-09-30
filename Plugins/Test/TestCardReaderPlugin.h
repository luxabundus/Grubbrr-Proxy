#pragma once


class TestCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	TestCardReaderPlugin();

	virtual void init(const ProxyStringMap &paramMap);
	virtual void exit();

	virtual void getStatus(ProxyStringMap &statusMap);

	virtual void sendPayment(const TransactionRequest &request, TransactionResponse &response);
	virtual void sendRefund(const TransactionRequest &request, TransactionResponse &response);
};

