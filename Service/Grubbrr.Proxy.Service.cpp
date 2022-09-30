#include "pch.h"
#include "ProxyApiServer.h"
#include "ProxyAdminServer.h"


class GrubbrrProxyServiceApp : public ServiceApp
{
public:
	GrubbrrProxyServiceApp() :
		ServiceApp("Grubbrr Proxy Service")
	{
	}

protected:
	virtual bool initApp()
	{
		if (!ServiceApp::initApp())
		{
			return false;
		}

		AfxLogInfo("Initializing model");
		if (!m_model.init())
		{
			return false;
		}

		AfxLogInfo("Initializing admin");
		if (!m_admin.init(m_model))
		{
			return false;
		}

		AfxLogInfo("Initializing api");
		if (!m_api.init(m_model))
		{
			return false;
		}

		return true;
	}

	virtual void exitApp()
	{
		m_api.stop();
		m_admin.stop();
		m_model.exit();
		ServiceApp::exitApp();
	}

private:
	ProxyModel m_model;
	ProxyApiServer m_api;
	ProxyAdminServer m_admin;
};

GrubbrrProxyServiceApp theApp;