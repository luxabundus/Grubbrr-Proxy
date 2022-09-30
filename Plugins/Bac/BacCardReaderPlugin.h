#pragma once
#include <Common/Includes.h>


class BacCardReaderWrapper;

class BacCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	BacCardReaderPlugin();

	virtual void init(const ProxyStringMap &paramMap);
	virtual void exit();

	virtual void getStatus(ProxyStringMap &statusMap);

	virtual void sendPayment(const TransactionRequest &request, TransactionResponse &response);
	virtual void sendRefund(const TransactionRequest &request, TransactionResponse &response);

private:
	BacCardReaderWrapper *m_pWrapper;
};
