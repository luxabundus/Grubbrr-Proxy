#pragma once
#include <Common/ProxyPlugin.h>


class GRUBBRR_PROXY_COMMON_DECLSPEC ProxyCardReaderPlugin : public ProxyPlugin
{
public:
	struct TransactionRequest
	{
		ProxyString terminalId;
		ProxyString orderId;
		double subTotal;
		double taxTotal;
		bool settle;

		TransactionRequest() :
			subTotal(0),
			taxTotal(0),
			settle(false)
		{
		}
	};
	struct TransactionResponse
	{
		ProxyString status;
		ProxyString statusCode;
		ProxyString statusMessage;
		ProxyString transactionId;
		ProxyString cardScheme;
	};

	ProxyCardReaderPlugin();

	virtual void sendPayment(const TransactionRequest &request, TransactionResponse &response) = 0;
	virtual void sendRefund(const TransactionRequest &request, TransactionResponse &response) = 0;
};
