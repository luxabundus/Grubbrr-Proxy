#include "BacCardReaderPlugin.h"


DEFINE_PROXY_PLUGIN(BacCardReaderPlugin)


BacCardReaderPlugin::BacCardReaderPlugin()
{
	m_pRequest = gcnew EMVStreamRequest();
}


void BacCardReaderPlugin::init(const ProxyStringMap &paramMap)
{
}


void BacCardReaderPlugin::exit()
{
}


ProxyStringMap BacCardReaderPlugin::sendPayment(ProxyStringMap &request)
{
	ProxyStringMap response;
	return response;
}


ProxyStringMap BacCardReaderPlugin::sendRefund(ProxyStringMap &request)
{
	ProxyStringMap response;
	return response;
}


ProxyStringMap BacCardReaderPlugin::settleTransactions(ProxyStringMap &settings)
{
	ProxyStringMap response;
	return response;
}


ProxyStringMap BacCardReaderPlugin::queryStatus()
{
	ProxyStringMap response;
	return response;
}

