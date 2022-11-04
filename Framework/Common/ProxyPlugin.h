#pragma once
#include <Common/Framework.h>


class GRUBBRR_PROXY_COMMON_DECLSPEC ProxyPlugin
{
public:
	struct Exception
	{
		int code;
		ProxyString message;

		Exception(Exception &&origin) noexcept;
		Exception(const Exception &origin);
		Exception(int code, const ProxyString &message);
	};

	struct Transaction
	{
		ProxyString status;
		ProxyString statusCode;
		ProxyString statusMessage;
		ProxyStringMap requestData;
		ProxyStringMap resultData;
	};

	ProxyPlugin();
	virtual ~ProxyPlugin();

	virtual void init(const ProxyStringMap &paramMap) = 0;
	virtual void exit() = 0;

	virtual void queryStatus(Transaction &transaction) = 0;

	static ProxyPlugin *Load(const char *className, const char *filePath, ProxyStringMap &configParams);
	static void Unload(ProxyPlugin *pPlugin);

private:
	HMODULE m_hModule;
};


inline ProxyPlugin::Exception::Exception(Exception &&origin) noexcept :
	code(std::exchange(origin.code, 0)),
	message(std::move(origin.message))
{
}

inline ProxyPlugin::Exception::Exception(const Exception &origin) :
	code(origin.code),
	message(origin.message)
{
}

inline ProxyPlugin::Exception::Exception(int _code, const ProxyString &_message) :
	code(_code),
	message(_message)
{
}



#define DEFINE_PROXY_PLUGIN(ClassName) \
extern "C" __declspec(dllexport) ProxyPlugin* Proxy_Create##ClassName() { return new ClassName; }

