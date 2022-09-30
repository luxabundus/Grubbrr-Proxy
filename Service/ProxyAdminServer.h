#pragma once


class ProxyAdminServer : public ProxyServer
{
public:
	ProxyAdminServer();

	bool initServer(RegKey &settings);

protected:
	DECLARE_HTTP_MAP()
};

