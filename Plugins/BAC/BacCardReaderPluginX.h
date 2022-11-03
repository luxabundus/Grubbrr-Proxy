#pragma once
#include "EMVStreamWrapper.h"


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
	virtual void settlePayments(Transaction &transaction);

private:
	gcroot<EMVStreamRequestWrapper^> m_wrapper;

	void validateRequestData(Transaction &transaction, const char *fieldName);

	EMVStreamRequest ^createEmvRequest(const char *type, Transaction &transaction);

	void prepareSaleRequest(Transaction &transaction, );
	void getResponseStatus(Transaction &transaction, XmlDocument ^result);
	bool getXmlValue(ProxyString &value, XmlDocument ^xml, String ^xmlName);
	bool getXmlValue(ProxyStringMap &response, const char *respName, XmlDocument ^xml, String ^xmlName);
};
