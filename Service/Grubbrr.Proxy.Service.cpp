#include "pch.h"
#include "ProxyApiProvider.h"
#include "ProxyAdminProvider.h"


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

	virtual bool initLogging()
	{
		// Initialize as a service...
		if (AfxGetSessionId() == 0)
		{
			if (!__super::initLogging())
			{
				return false;
			}

			AfxAttachLogFileSink();
		}
		// or as a console.
		else if (!ConsoleApp::initApp())
		{
			return false;
		}

		return true;
	}

	virtual void exitApp()
	{
		m_api.stop(5000);
		m_admin.stop(5000);
		m_model.exit();
		ServiceApp::exitApp();
	}

private:
	ProxyModel m_model;
	ProxyApiProvider m_api;
	ProxyAdminProvider m_admin;
};

GrubbrrProxyServiceApp theApp;