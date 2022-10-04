#pragma once
#include <Common/ProxyPlugin.h>


class GRUBBRR_PROXY_COMMON_DECLSPEC ProxyCardReaderPlugin : public ProxyPlugin
{
public:
	ProxyCardReaderPlugin();

	virtual ProxyStringMap sendPayment(ProxyStringMap &request) = 0;
	virtual ProxyStringMap sendRefund(ProxyStringMap &request) = 0;
	virtual ProxyStringMap settleTransactions(ProxyStringMap &settings) = 0;
};
