#pragma once
#include <Common/ProxyPlugin.h>


class GRUBBRR_PROXY_COMMON_DECLSPEC ProxyCardReaderPlugin : public ProxyPlugin
{
public:
	ProxyCardReaderPlugin();

	virtual ProxyStringMap sendPayment(const ProxyStringMap &request) = 0;
	virtual ProxyStringMap sendRefund(const ProxyStringMap &request) = 0;
	virtual ProxyStringMap settlePayments(const ProxyStringMap &settings) = 0;
};
