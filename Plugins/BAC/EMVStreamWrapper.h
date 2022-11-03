#pragma once

#include <vcclr.h>
#include <msclr\auto_gcroot.h>
#include <Common\Includes.h>

#using <System.Xml.dll>
#using "Runtime\CSP.EMV.InteropStream.dll"

using namespace System;
using namespace System::Reflection;
using namespace System::Xml;
using namespace CSP::EMV::InteropStream;


ref class EMVStreamRequestWrapper
{
public:
	XmlDocument ^queryStatus()
	{
		EMVStreamRequest request;
		request.transactionType = "ECHO_TEST";

		return loadXml(request.sendData());
	}

	XmlDocument ^sendPayment(ProxyCardReaderPlugin::Transaction &transaction)
	{
		EMVStreamRequest request;
		request.transactionType = "SALE";
		request.terminalId = _str(transaction.requestData["terminalId"]);
		request.invoice = _str(transaction.requestData["orderId"]);
		request.totalAmount = _str(transaction.requestData["totalAmount"]);

		return loadXml(request.sendData());
	}

	XmlDocument ^sendRefund(ProxyCardReaderPlugin::Transaction &transaction)
	{
		EMVStreamRequest request;
		request.transactionType = "REFUND";
		request.terminalId = _str(transaction.requestData["terminalId"]);
		request.invoice = _str(transaction.requestData["orderId"]);
		request.totalAmount = _str(transaction.requestData["totalAmount"]);

		parseTransactionId(transaction, request.authorizationNumber, nullptr, nullptr);

		return loadXml(request.sendData());
	}

	XmlDocument ^sendVoid(ProxyCardReaderPlugin::Transaction &transaction)
	{
		EMVStreamRequest request;
		request.transactionType = "VOID";
		request.terminalId = _str(transaction.requestData["terminalId"]);

		parseTransactionId(transaction, request.authorizationNumber, request.referenceNumber, request.systemTraceNumber);

		return loadXml(request.sendData());
	}

	XmlDocument ^sendReversion(ProxyCardReaderPlugin::Transaction &transaction)
	{
		EMVStreamRequest request;
		request.transactionType = "VOID";
		request.terminalId = _str(transaction.requestData["terminalId"]);
		request.invoice = _str(transaction.requestData["orderId"]);

		return loadXml(request.sendData());
	}

	XmlDocument ^settlePayments(ProxyCardReaderPlugin::Transaction &transaction)
	{
		EMVStreamRequest request;
		request.transactionType = "BATCH_SETTLEMENT";
		request.terminalId = _str(transaction.requestData["terminalId"]);

		return loadXml(request.sendData());
	}

private:
	String^ _str(const ProxyString &proxyString)
	{
		return proxyString ? gcnew String((const char*)proxyString) : nullptr;
	}

	void parseTransactionId(ProxyCardReaderPlugin::Transaction &transaction, String ^authorizationNumber, String ^referenceId, String ^systemTraceNumber)
	{
		String ^transactionId = _str(transaction.requestData["transactionId"]);
		array<String^> ^elements = transactionId->Split(':', 3);

		if (elements)
		{
			if ((elements->Length >= 1) && authorizationNumber)
			{
				authorizationNumber = elements[0];
			}
			if ((elements->Length >= 2) && referenceId)
			{
				referenceId = elements[1];
			}
			if ((elements->Length >= 1) && systemTraceNumber)
			{
				systemTraceNumber = elements[2];
			}
		}
	}

	XmlDocument ^loadXml(String ^wrapperResult)
	{
		XmlDocument ^xml = gcnew XmlDocument;
		xml->LoadXml(wrapperResult);
		return xml;
	}
};

