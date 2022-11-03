#include "BacCardReaderPlugin.h"
#include <msclr\marshal.h>
#include "EMVStreamWrapper.h"


#define HTTP_STATUS_BAD_REQUEST 400


DEFINE_PROXY_PLUGIN(BacCardReaderPlugin)


static inline ProxyString _prxstr(String ^source)
{
	msclr::interop::marshal_context ctx;
	return ctx.marshal_as<const char*>(source);
}



BacCardReaderPlugin::BacCardReaderPlugin()
{
}


void BacCardReaderPlugin::init(const ProxyStringMap &params)
{
	m_wrapper = gcnew EMVStreamRequestWrapper();
}

void BacCardReaderPlugin::exit()
{
}


void BacCardReaderPlugin::queryStatus(Transaction &transaction)
{
	XmlDocument ^result = m_wrapper->queryStatus();

	getResponseStatus(transaction, result);

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}


void BacCardReaderPlugin::sendPayment(Transaction &transaction)
{
	prepareSaleRequest(transaction);

	XmlDocument ^result = m_wrapper->sendPayment(transaction);

	getResponseStatus(transaction, result);

	getXmlValue(transaction.resultData, "transactionId", result, "transactionId");
	getXmlValue(transaction.resultData, "referenceNumber", result, "referenceNumber");
	getXmlValue(transaction.resultData, "systemTraceNumber", result, "systemTraceNumber");

	// Extract card-scheme.
	getXmlValue(transaction.resultData, "cardNumber", result, "maskedCardNumber");

	XmlNode ^printInfo = result->SelectSingleNode(String::Format("/printTags"));
	if (printInfo)
	{
		msclr::interop::marshal_context ctx;
		transaction.resultData["cardScheme"] = _prxstr(printInfo->FirstChild->InnerText);
	}

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}


void BacCardReaderPlugin::sendRefund(Transaction &transaction)
{
	validateRequestData(transaction, "transactionId");

	prepareSaleRequest(transaction);

	XmlDocument ^result = m_wrapper->sendRefund(transaction);

	getResponseStatus(transaction, result);

	getXmlValue(transaction.resultData, "cardHolder", result, "cardHolderName");

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}


void BacCardReaderPlugin::sendVoid(Transaction &transaction)
{
	validateRequestData(transaction, "transactionId");

	prepareSaleRequest(transaction);

	XmlDocument ^result = m_wrapper->sendVoid(transaction);

	getResponseStatus(transaction, result);

	getXmlValue(transaction.resultData, "cardHolder", result, "cardHolderName");

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}


void BacCardReaderPlugin::settlePayments(Transaction &transaction)
{
	XmlDocument ^result = m_wrapper->settlePayments(transaction);

	getResponseStatus(transaction, result);

	transaction.resultData["raw"] = _prxstr(result->OuterXml);
}


void BacCardReaderPlugin::validateRequestData(Transaction &transaction, const char *fieldName)
{
	if (!transaction.requestData.contains(fieldName))
	{
		throw Exception(HTTP_STATUS_BAD_REQUEST, "missing request field: %s", fieldName);
	}
}

void BacCardReaderPlugin::prepareSaleRequest(Transaction &transaction)
{
	double totalAmount = atof(transaction.requestData["totalAmount"]);

	double divisor = atof(transaction.requestData["divisor"]);
	if (!divisor)
	{
		divisor = 1;
	}

	transaction.requestData["totalAmount"].format("%0.2f", totalAmount / divisor);
}


void BacCardReaderPlugin::getResponseStatus(Transaction &transaction, XmlDocument ^result)
{
	ProxyStringMap response;
 	getXmlValue(transaction.status, result, "responseCodeDescription");
	getXmlValue(transaction.statusCode, result, "responseCode");
	getXmlValue(transaction.statusMessage, result, "errorDescription");

	getXmlValue(transaction.resultData, "transactionId", result, "transactionId");
	getXmlValue(transaction.resultData, "referenceNumber", result, "referenceNumber");
	getXmlValue(transaction.resultData, "systemTraceNumber", result, "systemTraceNumber");
}


bool BacCardReaderPlugin::getXmlValue(ProxyString &value, XmlDocument ^xml, String ^xmlName)
{
	XmlNode ^node = xml->SelectSingleNode(String::Format("/EMVStreamResponse/{0}", xmlName));
	if (node)
	{
		value = _prxstr(node->InnerText);
		return true;
	}

	return false;
}

bool BacCardReaderPlugin::getXmlValue(ProxyStringMap &response, const char *respName, XmlDocument ^xml, String ^xmlName)
{
	XmlNode ^node = xml->SelectSingleNode(String::Format("/EMVStreamResponse/{0}", xmlName));
	if (node)
	{
		response[respName] = _prxstr(node->InnerText);
		return true;
	}

	return false;
}
