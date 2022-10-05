#include "EMVStreamWrapper.h"
#include <msclr/marshal.h>


inline String^ _NetString(const ProxyString &proxyString)
{
	return gcnew String((const char*)proxyString);
}


ProxyStringMap EMVStreamRequestWrapper::sendPayment(ProxyStringMap &params)
{
	msclr::interop::marshal_context ctx;

	m_request.transactionType = "SALE";
	m_request.terminalId = _NetString(params["terminalId"]);
	m_request.totalAmount = _NetString(params["subTotal"]);
	m_request.invoice = _NetString(params["orderId"]);

	ProxyStringMap results;
	results["xml"] = ctx.marshal_as<const char*>(m_request.sendData());
	return results;
}


ProxyStringMap EMVStreamRequestWrapper::sendRefund(ProxyStringMap &params)
{
	m_request.transactionType = "REFUND";
	m_request.terminalId = _NetString(params["terminalId"]);
	m_request.totalAmount = _NetString(params["subTotal"]);
	m_request.invoice = _NetString(params["orderId"]);
	m_request.authorizationNumber = _NetString(params["authId"]);

	ProxyStringMap results;
	msclr::interop::marshal_context ctx;
	results["xml"] = ctx.marshal_as<const char*>(m_request.sendData());
	return results;
}


ProxyStringMap EMVStreamRequestWrapper::settlePayments(ProxyStringMap &params)
{
	m_request.transactionType = "BATCH_SETTLEMENT";
	m_request.terminalId = _NetString(params["terminalId"]);

	ProxyStringMap results;
	msclr::interop::marshal_context ctx;
	results["xml"] = ctx.marshal_as<const char*>(m_request.sendData());
	return results;
}


ProxyStringMap EMVStreamRequestWrapper::queryStatus()
{
	m_request.transactionType = "ECHO_TEST";

	ProxyStringMap results;
	msclr::interop::marshal_context ctx;
	results["xml"] = ctx.marshal_as<const char*>(m_request.sendData());
	return results;
}

