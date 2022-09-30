#pragma once


class ProxyServer : public HttpServer
{
public:
	ProxyServer(const String &name);

	String getName();
	ProxyModel &getModel();

	bool init(ProxyModel &model);

protected:
	virtual bool initServer(RegKey &settings);

private:
	String m_name;
	ProxyModel *m_pModel;
};


inline String ProxyServer::getName()
{
	return m_name;
}

inline ProxyModel &ProxyServer::getModel()
{
	return *m_pModel;
}
