#pragma once


class GRUBBRR_PROXY_COMMON_DECLSPEC ProxyStringMap
{
	using Entry = std::pair<ProxyString, ProxyString>;
	using EntryVector = std::vector<Entry>;
	using KeyMap = std::map<ProxyString, size_t>;

public:
	ProxyStringMap();
	ProxyStringMap(ProxyStringMap &&origin) noexcept;
	ProxyStringMap(const ProxyStringMap &origin);
	virtual ~ProxyStringMap();

	size_t size() const
	{
		return m_pEntries->size();
	}

	ProxyString operator [] (const char *pKey) const;
	ProxyString &operator [] (const char *pKey);

	ProxyStringMap &operator = (ProxyStringMap &&origin);
	ProxyStringMap &operator = (const ProxyStringMap &origin);

	using iterator = std::vector<Entry>::iterator;
	using const_iterator = std::vector<Entry>::const_iterator;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	bool contains(const char *pKey) const;
	bool lookup(const char *pKey, ProxyString &value) const;

private:
	void assign(const char *pKey, const char *pValue);
	void move(ProxyStringMap &origin) noexcept;
	void copy(const ProxyStringMap &origin);

private:
	EntryVector *m_pEntries;
	KeyMap *m_pKeys;
};


inline ProxyStringMap::ProxyStringMap(ProxyStringMap &&origin) noexcept
{
	move(origin);
}

inline ProxyStringMap::ProxyStringMap(const ProxyStringMap &origin)
{
	copy(origin);
}

inline ProxyStringMap &ProxyStringMap::operator = (ProxyStringMap &&origin)
{
	move(origin);
	return *this;
}

inline ProxyStringMap &ProxyStringMap::operator = (const ProxyStringMap &origin)
{
	copy(origin);
	return *this;
}
