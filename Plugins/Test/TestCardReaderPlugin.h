#pragma once


class TestCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	TestCardReaderPlugin();

	virtual void init(const ProxyStringMap &params);
	virtual void exit();

	virtual void queryStatus(Transaction &transactions);

	virtual void sendPayment(Transaction &transactions);
	virtual void sendRefund(Transaction &transactions);
	virtual void sendVoid(Transaction &transactions);
	virtual void sendSettlement(Transaction &transactions);
};

