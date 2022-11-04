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


class BacCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	BacCardReaderPlugin();

	virtual void init(const ProxyStringMap &params);
	virtual void exit();

	virtual void queryStatus(Transaction &transaction);
	virtual void sendPayment(Transaction &transaction);
	virtual void sendRefund(Transaction &transaction);
	virtual void sendVoid(Transaction &transaction);
	virtual void sendSettlement(Transaction &transaction);

private:
	XmlDocument ^execute(Transaction &transaction, EMVStreamRequest ^request);
	void reverse(const Transaction &transaction);

	ProxyString formatTransactionId(XmlNode ^xml);
	void parseTransactionId(const ProxyString &transactionId, EMVStreamRequest ^request);
	bool getXmlValue(ProxyString &value, XmlDocument ^xml, const char *xmlName);
};
