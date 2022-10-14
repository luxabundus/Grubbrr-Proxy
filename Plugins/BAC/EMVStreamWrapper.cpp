#include "EMVStreamWrapper.h"

/*
inline String^ _str(const ProxyString &proxyString)
{
	return gcnew String((const char*)proxyString);
}


void EMVStreamRequestWrapper::init(const ProxyStringMap &params)
{
	// Need code to initialize component and card-reader.
}


String^ EMVStreamRequestWrapper::sendPayment(const ProxyStringMap &params)
{
	EMVStreamRequest request;
	request.transactionType = "SALE";
	request.terminalId = _str(params["terminalId"]);
	request.totalAmount = _str(params["subTotal"]);
	request.invoice = _str(params["orderId"]);
	return request.sendData();
}


String^ EMVStreamRequestWrapper::sendRefund(const ProxyStringMap &params)
{
	EMVStreamRequest request;
	request.transactionType = "REFUND";
	request.terminalId = _str(params["terminalId"]);
	request.totalAmount = _str(params["subTotal"]);
	request.invoice = _str(params["orderId"]);
	request.authorizationNumber = _str(params["authId"]);
	return request.sendData();
}


String^ EMVStreamRequestWrapper::settlePayments(const ProxyStringMap &params)
{
	EMVStreamRequest request;
	request.transactionType = "BATCH_SETTLEMENT";
	request.terminalId = _str(params["terminalId"]);
	return request.sendData();
}


String^ EMVStreamRequestWrapper::queryStatus()
{
	EMVStreamRequest request;
	request.transactionType = "ECHO_TEST";
	return request.sendData();
}
*/