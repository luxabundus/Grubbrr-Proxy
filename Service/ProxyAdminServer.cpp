#include "pch.h"
#include "ProxyAdminServer.h"


BEGIN_HTTP_MAP(ProxyAdminServer)
	ON_HTTP_DEFAULT_FILE("/", "AdminUI", "admin.htm", "htm")
END_HTTP_MAP()


ProxyAdminServer::ProxyAdminServer() :
	ProxyServer("Admin")
{
}


bool ProxyAdminServer::initServer(RegKey &settings)
{
	if (!ProxyServer::initServer(settings))
	{
		return false;
	}

	String listener = settings.getStringValue("Listener");
	if (listener.isEmpty())
	{
		listener = "tcp:localhost:54321";
	}
	return startListener(listener);
}