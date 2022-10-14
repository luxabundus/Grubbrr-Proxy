#pragma once

#include <vcclr.h>
#include <msclr\auto_gcroot.h>
#include <Common\Includes.h>

#using "CSP.EMV.InteropStream.dll"

using namespace System;
using namespace System::Reflection;
using namespace CSP::EMV::InteropStream;


ref class EMVStreamRequestWrapper
{
public:
	void init(const ProxyStringMap &params)
	{
		// Need code to initialize component and card-reader.
	}

	String^ sendPayment(const ProxyStringMap &params)
	{
		EMVStreamRequest request;
		request.transactionType = "SALE";
		request.terminalId = _str(params["terminalId"]);
		request.totalAmount = _str(params["subTotal"]);
		request.invoice = _str(params["orderId"]);
		return request.sendData();
	}

	String^ sendRefund(const ProxyStringMap &params)
	{
		EMVStreamRequest request;
		request.transactionType = "REFUND";
		request.terminalId = _str(params["terminalId"]);
		request.totalAmount = _str(params["subTotal"]);
		request.invoice = _str(params["orderId"]);
		request.authorizationNumber = _str(params["authId"]);
		return request.sendData();
	}

	String^ settlePayments(const ProxyStringMap &params)
	{
		EMVStreamRequest request;
		request.transactionType = "BATCH_SETTLEMENT";
		request.terminalId = _str(params["terminalId"]);
		return request.sendData();
	}

	String^ queryStatus()
	{
		EMVStreamRequest request;
		request.transactionType = "ECHO_TEST";
		return request.sendData();
	}

private:
	String^ _str(const ProxyString &proxyString)
	{
		return gcnew String((const char*)proxyString);
	}
};

