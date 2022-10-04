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

		template <typename... Args> Exception(int code, const char *format, Args&...args);
	};

	ProxyPlugin();
	virtual ~ProxyPlugin();

	virtual void init(const ProxyStringMap &paramMap) = 0;
	virtual void exit() = 0;

	virtual ProxyStringMap queryStatus() = 0;

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

template <typename... Args>
inline ProxyPlugin::Exception::Exception(int _code, const char *fmt, Args&...args) :
	code(_code)
{
	message.format(fmt, args...);
}


#define DEFINE_PROXY_PLUGIN(ClassName) \
extern "C" __declspec(dllexport) ProxyPlugin* Proxy_Create##ClassName() { return new ClassName; }

