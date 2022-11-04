#include "BacCardReaderPlugin.h"
#include <msclr\marshal.h>


DEFINE_PROXY_PLUGIN(BacCardReaderPlugin)


static inline String^ _str(const ProxyString &proxyString)
{
	return proxyString ? gcnew String((const char*)proxyString) : nullptr;
}

static inline ProxyString _prxstr(String ^source)
{
	msclr::interop::marshal_context ctx;
	return ctx.marshal_as<const char*>(source);
}

static inline String ^_xmlstr(XmlNode ^xml, const char *name)
{
	XmlNode ^node = xml->SelectSingleNode(String::Format("/EMVStreamResponse/{0}", _str(name)));
	return node ? node->InnerText->Trim() : nullptr;
}

static inline ProxyString _xmlprxstr(XmlNode ^xml, const char *name)
{
	return _prxstr(_xmlstr(xml, name));
}



BacCardReaderPlugin::BacCardReaderPlugin()
{
}


void BacCardReaderPlugin::init(const ProxyStringMap &params)
{
}

void BacCardReaderPlugin::exit()
{
}


void BacCardReaderPlugin::queryStatus(Transaction &transaction)
{
	EMVStreamRequest ^request = gcnew EMVStreamRequest;
	request->transactionType = "ECHO_TEST";

	XmlDocument ^result = execute(transaction, request);

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}

void BacCardReaderPlugin::sendPayment(Transaction &transaction)
{
	EMVStreamRequest ^request = gcnew EMVStreamRequest;
	request->transactionType = "SALE";
	request->terminalId = _str(transaction.requestData["terminalId"]);
	request->invoice = _str(transaction.requestData["orderId"]);
	request->totalAmount = _str(transaction.requestData["totalAmount"]);

	XmlDocument ^result = execute(transaction, request);

	if (transaction.statusCode == "00")
	{
		transaction.resultData["transactionId"] = formatTransactionId(result);
		transaction.resultData["cardNumber"] = _xmlprxstr(result, "maskedCardNumber");
		transaction.resultData["cardScheme"] = _xmlprxstr(result, "/printTags/string");
	}
	else
	{
		reverse(transaction);
	}

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}

void BacCardReaderPlugin::sendRefund(Transaction &transaction)
{
	EMVStreamRequest ^request = gcnew EMVStreamRequest;
	request->transactionType = "REFUND";
	request->terminalId = _str(transaction.requestData["terminalId"]);
	request->invoice = _str(transaction.requestData["orderId"]);
	request->totalAmount = _str(transaction.requestData["totalAmount"]);

	parseTransactionId(transaction.requestData["transactionId"], request);

	XmlDocument ^result = execute(transaction, request);

	if (transaction.statusCode == "00")
	{
		transaction.resultData["transactionId"] = formatTransactionId(result);
	}
	else
	{
		reverse(transaction);
	}

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}

void BacCardReaderPlugin::sendVoid(Transaction &transaction)
{
	EMVStreamRequest ^request = gcnew EMVStreamRequest;
	request->transactionType = "VOID";
	request->terminalId = _str(transaction.requestData["terminalId"]);

	parseTransactionId(transaction.requestData["transactionId"], request);

	XmlDocument ^result = execute(transaction, request);

	if (transaction.statusCode == "00")
	{
		transaction.resultData["transactionId"] = formatTransactionId(result);
	}
	else
	{
		reverse(transaction);
	}

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}

void BacCardReaderPlugin::sendSettlement(Transaction &transaction)
{
	EMVStreamRequest ^request = gcnew EMVStreamRequest;
	request->transactionType = "BATCH_SETTLEMENT";
	request->terminalId = _str(transaction.requestData["terminalId"]);

	XmlDocument ^result = execute(transaction, request);

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}


XmlDocument ^BacCardReaderPlugin::execute(Transaction &transaction, EMVStreamRequest ^request)
{
	XmlDocument ^result = gcnew XmlDocument;
	result->LoadXml(request->sendData());

	getXmlValue(transaction.status, result, "responseCodeDescription");
	getXmlValue(transaction.statusCode, result, "responseCode");
	getXmlValue(transaction.statusMessage, result, "errorDescription");

	return result;
}

void BacCardReaderPlugin::reverse(const Transaction &transaction)
{
	EMVStreamRequest ^request = gcnew EMVStreamRequest;
	request->transactionType = "REVERSE";
	request->terminalId = _str(transaction.requestData["terminalId"]);
	request->invoice = _str(transaction.requestData["orderId"]);
	request->sendData();
}

ProxyString BacCardReaderPlugin::formatTransactionId(XmlNode ^xml)
{
	return _prxstr(String::Format("{0}-{1}-{2}", _xmlstr(xml, "authorizationNumber"), _xmlstr(xml, "referenceNumber"), _xmlstr(xml, "systemTraceNumber")));
}

void BacCardReaderPlugin::parseTransactionId(const ProxyString &transactionId, EMVStreamRequest ^request)
{
 	array<String^> ^elements = _str(transactionId)->Split('-');

	if (elements)
	{
		if (elements->Length >= 1)
		{
			request->authorizationNumber = elements[0];
		}
		if (elements->Length >= 2)
		{
			request->referenceNumber = elements[1];
		}
		if (elements->Length >= 3)
		{
			request->systemTraceNumber = elements[2];
		}
	}
}

bool BacCardReaderPlugin::getXmlValue(ProxyString &value, XmlDocument ^xml, const char *xmlName)
{
	XmlNode ^node = xml->SelectSingleNode(String::Format("/EMVStreamResponse/{0}", _str(xmlName)));
	if (node)
	{
		value = _prxstr(node->InnerText);
		return true;
	}

	return false;
}
