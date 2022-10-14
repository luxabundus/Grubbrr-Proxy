#pragma once


class TestCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	TestCardReaderPlugin();

	virtual void init(const ProxyStringMap &params);
	virtual void exit();

	virtual ProxyStringMap sendPayment(const ProxyStringMap &params);
	virtual ProxyStringMap sendRefund(const ProxyStringMap &params);
	virtual ProxyStringMap settlePayments(const ProxyStringMap &params);
	virtual ProxyStringMap queryStatus();
};

