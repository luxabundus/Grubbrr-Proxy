#pragma once


class GRUBBRR_PROXY_COMMON_DECLSPEC ProxyString
{
public:
	ProxyString();
	ProxyString(ProxyString &&origin) noexcept;
	ProxyString(const ProxyString &origin);
	ProxyString(const char *origin);
	ProxyString(const std::string &origin);
	~ProxyString();

	ProxyString &operator = (ProxyString &&origin) noexcept;
	ProxyString &operator = (const ProxyString &origin);
	ProxyString &operator = (const char *origin);
	ProxyString &operator = (const std::string &origin);

	bool operator < (const char *string) const;
	bool operator == (const char *string) const;

	operator char * ();
	operator const char * () const;

	bool empty() const;
	const char *data() const;

	template<typename ...Args> ProxyString &format(const char *fmt, Args &... args);

private:
	char *m_string;

	void copy(const char *string);
	void clear();
};


inline ProxyString::ProxyString() :
	m_string(nullptr)
{
}

inline ProxyString::ProxyString(ProxyString &&origin) noexcept :
	m_string(std::exchange(origin.m_string, nullptr))
{
}

inline ProxyString::ProxyString(const ProxyString &origin) :
	m_string(nullptr)
{
	copy(origin.m_string);
}

inline ProxyString::ProxyString(const char *origin) :
	m_string(nullptr)
{
	copy(origin);
}

inline ProxyString::ProxyString(const std::string &origin) :
	m_string(nullptr)
{
	copy(origin.data());
}

inline ProxyString::~ProxyString()
{
	clear();
}

inline ProxyString &ProxyString::operator = (ProxyString &&origin) noexcept
{
	clear();
	m_string = std::exchange(origin.m_string, nullptr);
	return *this;
}

inline ProxyString &ProxyString::operator = (const ProxyString &origin)
{
	copy(origin.m_string);
	return *this;
}

inline ProxyString &ProxyString::operator = (const char *string)
{
	copy(string);
	return *this;
}

inline ProxyString &ProxyString::operator = (const std::string &string)
{
	copy(string.data());
	return *this;
}

inline bool ProxyString::operator < (const char *string) const
{
	return std::strcmp(m_string ? m_string : "", string) < 0;
}

inline bool ProxyString::operator == (const char *string) const
{
	return std::strcmp(m_string ? m_string : "", string) == 0;
}

inline ProxyString::operator char * ()
{
	return m_string;
}
inline ProxyString::operator const char * () const
{
	return m_string;
}

inline bool ProxyString::empty() const
{
	return !m_string || !*m_string;
}

inline const char *ProxyString::data() const
{
	return m_string ? m_string : "";
}


template<typename ...Args>
ProxyString &ProxyString::format(const char *fmt, Args &... args)
{
	size_t length = snprintf(nullptr, 0, fmt, args ...);
	if (length != 0)
	{
		m_string = new char[length + 1];
		snprintf(m_string, length + 1, fmt, args ...);
	}
	return *this;
}