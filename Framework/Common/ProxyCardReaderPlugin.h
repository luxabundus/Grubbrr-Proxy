#pragma once
#include <Common/ProxyPlugin.h>


class GRUBBRR_PROXY_COMMON_DECLSPEC ProxyCardReaderPlugin : public ProxyPlugin
{
public:
	ProxyCardReaderPlugin();

	virtual void sendPayment(Transaction &transaction) = 0;
	virtual void sendRefund(Transaction &transaction) = 0;
	virtual void sendVoid(Transaction &transaction) = 0;
	virtual void settlePayments(Transaction &transaction) = 0;
};
