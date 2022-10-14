#include "BacCardReaderPlugin.h"
#include "EMVStreamWrapper.h"
#include <msclr\marshal.h>


DEFINE_PROXY_PLUGIN(BacCardReaderPlugin)


BacCardReaderPlugin::BacCardReaderPlugin()
{
}


void BacCardReaderPlugin::init(const ProxyStringMap &params)
{
	m_wrapper = gcnew EMVStreamRequestWrapper();
	m_wrapper->init(params);
}


void BacCardReaderPlugin::exit()
{
}


ProxyStringMap BacCardReaderPlugin::sendPayment(const ProxyStringMap &params)
{
	ProxyStringMap results;
	msclr::interop::marshal_context ctx;
	results["xml"] = ctx.marshal_as<const char*>(m_wrapper->sendPayment(params));
	return results;
}


ProxyStringMap BacCardReaderPlugin::sendRefund(const ProxyStringMap &params)
{
	ProxyStringMap results;
	msclr::interop::marshal_context ctx;
	results["xml"] = ctx.marshal_as<const char*>(m_wrapper->sendRefund(params));
	return results;
}


ProxyStringMap BacCardReaderPlugin::settlePayments(const ProxyStringMap &params)
{
	ProxyStringMap results;
	msclr::interop::marshal_context ctx;
	results["xml"] = ctx.marshal_as<const char*>(m_wrapper->settlePayments(params));
	return results;
}


ProxyStringMap BacCardReaderPlugin::queryStatus()
{
	ProxyStringMap results;
	msclr::interop::marshal_context ctx;
	results["xml"] = ctx.marshal_as<const char*>(m_wrapper->queryStatus());
	return results;
}

