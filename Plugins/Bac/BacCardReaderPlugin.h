#pragma once
#include "EMVStreamWrapper.h"


class BacCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	BacCardReaderPlugin();

	virtual void init(const ProxyStringMap &paramMap);
	virtual void exit();

	virtual ProxyStringMap sendPayment(ProxyStringMap &request);
	virtual ProxyStringMap sendRefund(ProxyStringMap &request);
	virtual ProxyStringMap settleTransactions(ProxyStringMap &settings);
	virtual ProxyStringMap queryStatus();

private:
	gcroot<EMVStreamRequestWrapper^> m_request;
};
