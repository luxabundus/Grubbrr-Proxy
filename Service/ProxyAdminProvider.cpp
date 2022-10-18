#include "pch.h"
#include "ProxyAdminProvider.h"


BEGIN_HTTP_MAP(ProxyAdminProvider)
	ON_HTTP_DEFAULT_FILE("/", "AdminUI", "admin.htm", "htm")
END_HTTP_MAP()


ProxyAdminProvider::ProxyAdminProvider() :
	ProxyProvider("Admin")
{
}


bool ProxyAdminProvider::initServer(RegKey &settings)
{
	if (!ProxyProvider::initServer(settings))
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