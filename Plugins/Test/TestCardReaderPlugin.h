#pragma once


class TestCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	TestCardReaderPlugin();

	virtual void init(const ProxyStringMap &paramMap);
	virtual void exit();

	virtual ProxyStringMap sendPayment(ProxyStringMap &request);
	virtual ProxyStringMap sendRefund(ProxyStringMap &request);
	virtual ProxyStringMap settlePayments(ProxyStringMap &settings);
	virtual ProxyStringMap queryStatus();
};

