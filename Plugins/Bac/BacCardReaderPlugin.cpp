#include "BacCardReaderPlugin.h"
#include "EMVStreamWrapper.h"


DEFINE_PROXY_PLUGIN(BacCardReaderPlugin)


BacCardReaderPlugin::BacCardReaderPlugin()
{
}


void BacCardReaderPlugin::init(const ProxyStringMap &paramMap)
{
	m_request = gcnew EMVStreamRequestWrapper();
}


void BacCardReaderPlugin::exit()
{
}


ProxyStringMap BacCardReaderPlugin::sendPayment(ProxyStringMap &params)
{
	ProxyStringMap responseMap;

	responseMap["data"] = m_request->sendPayment(params);

	return responseMap;
}


ProxyStringMap BacCardReaderPlugin::sendRefund(ProxyStringMap &params)
{
	ProxyStringMap response;
	return response;
}


ProxyStringMap BacCardReaderPlugin::settleTransactions(ProxyStringMap &params)
{
	ProxyStringMap response;
	return response;
}


ProxyStringMap BacCardReaderPlugin::queryStatus()
{
	ProxyStringMap response;
	return response;
}

