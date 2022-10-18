#pragma once


class ProxyAdminProvider : public ProxyProvider
{
public:
	ProxyAdminProvider();

	bool initServer(RegKey &settings);

protected:
	DECLARE_HTTP_MAP()
};

