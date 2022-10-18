#pragma once


class ProxyProvider : public HttpServer
{
public:
	ProxyProvider(const String &name);

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
		Accessor(ProxyProvider *pServer, HttpServerContext &context);

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


inline String ProxyProvider::getName()
{
	return m_name;
}

inline ProxyModel &ProxyProvider::getModel()
{
	return *m_pModel;
}
