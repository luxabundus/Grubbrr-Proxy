#include "BacCardReaderPlugin.h"
#include "EMVStreamWrapper.h"


DEFINE_PROXY_PLUGIN(BacCardReaderPlugin)


BacCardReaderPlugin::BacCardReaderPlugin()
{
}


void BacCardReaderPlugin::init(const ProxyStringMap &paramMap)
{
	m_wrapper = gcnew EMVStreamRequestWrapper();
}


void BacCardReaderPlugin::exit()
{
}


ProxyStringMap BacCardReaderPlugin::sendPayment(ProxyStringMap &params)
{
	return m_wrapper->sendPayment(params);
}


ProxyStringMap BacCardReaderPlugin::sendRefund(ProxyStringMap &params)
{
	return m_wrapper->sendRefund(params);
}


ProxyStringMap BacCardReaderPlugin::settlePayments(ProxyStringMap &params)
{
	return m_wrapper->settlePayments(params);
}


ProxyStringMap BacCardReaderPlugin::queryStatus()
{
	return m_wrapper->queryStatus();
}

