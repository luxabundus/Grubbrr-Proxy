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


void BacCardReaderPlugin::getStatus(ProxyStringMap &statusMap)
{
}


void BacCardReaderPlugin::sendPayment(const TransactionRequest &request, TransactionResponse &response)
{
	m_pRequest->sendData();
}


void BacCardReaderPlugin::sendRefund(const TransactionRequest &request, TransactionResponse &response)
{
}
