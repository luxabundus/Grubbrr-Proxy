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

	static ProxyStringMap JsonToProxy(const Json &json);
	static Json ProxyToJson(const ProxyStringMap &proxy);

protected:
	friend class ApiAccessor;

	class Accessor
	{
	public:
		Accessor(ProxyServer *pServer, HttpServerContext &context);

		Json getRequest(bool throwIfUndefined = true);
		void setResponse(Json &apiResponse);

	protected:
		HttpServerContext &m_context;
	};

private:
	String m_name;
	ProxyModel *m_pModel;

	String m_authUser;
	String m_authPassword;
};


inline String ProxyServer::getName()
{
	return m_name;
}

inline ProxyModel &ProxyServer::getModel()
{
	return *m_pModel;
}
