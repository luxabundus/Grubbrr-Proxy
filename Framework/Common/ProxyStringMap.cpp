#include "pch.h"
#include "ProxyStringMap.h"



ProxyStringMap::ProxyStringMap() :
	m_pEntries(new EntryVector),
	m_pKeys(new KeyMap)
{
}

ProxyStringMap::~ProxyStringMap()
{
	delete m_pKeys;
	delete m_pEntries;
}


ProxyString ProxyStringMap::operator [] (const char *pKey) const
{
	auto res = m_pKeys->find(pKey);
	return res == m_pKeys->end() ? nullptr : m_pEntries->at(res->second).second;
}

ProxyString &ProxyStringMap::operator [] (const char *pKey)
{
	auto res = m_pKeys->find(pKey);
	if (res == m_pKeys->end())
	{
		size_t index = m_pEntries->size();
		m_pEntries->emplace_back(pKey, nullptr);

		Entry &entry = m_pEntries->back();
		m_pKeys->emplace(entry.first, index);

		return entry.second;
	}
	else
	{
		return m_pEntries->at(res->second).second;
	}
}


ProxyStringMap::iterator ProxyStringMap::begin()
{
	return m_pEntries->begin();
}

ProxyStringMap::const_iterator ProxyStringMap::begin() const
{
	return m_pEntries->begin();
}

ProxyStringMap::iterator ProxyStringMap::end()
{
	return m_pEntries->end();
}

ProxyStringMap::const_iterator ProxyStringMap::end() const
{
	return m_pEntries->end();
}


bool ProxyStringMap::contains(const char *pKey) const
{
	return m_pKeys->find(pKey) != m_pKeys->end();
}


bool ProxyStringMap::lookup(const char *pKey, ProxyString &value) const
{
	bool found = false;

	auto res = m_pKeys->find(pKey);
	if ((found = res != m_pKeys->end()))
	{
		value = m_pEntries->at(res->second).second;
	}
	
	return found;
}


void ProxyStringMap::move(ProxyStringMap &origin) noexcept
{
	m_pEntries = std::exchange(origin.m_pEntries, nullptr);
	m_pKeys = std::exchange(origin.m_pKeys, nullptr);
}


void ProxyStringMap::copy(const ProxyStringMap &origin)
{
	size_t index = 0;

	m_pEntries = new EntryVector(*origin.m_pEntries);

	for (auto it : *m_pEntries)
	{
		m_pKeys->at(it.first) = index++;
	}
}


