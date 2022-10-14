#pragma once
#include "EMVStreamWrapper.h"


class BacCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	BacCardReaderPlugin();

	virtual void init(const ProxyStringMap &params);
	virtual void exit();

	virtual ProxyStringMap sendPayment(const ProxyStringMap &params);
	virtual ProxyStringMap sendRefund(const ProxyStringMap &params);
	virtual ProxyStringMap settlePayments(const ProxyStringMap &params);
	virtual ProxyStringMap queryStatus();

private:
	gcroot<EMVStreamRequestWrapper^> m_wrapper;
};
