#include "pch.h"
#include "ProxyString.h"


void ProxyString::copy(const char *string)
{
	clear();
	m_string = ::_strdup(string);
}


void ProxyString::clear()
{
	if (m_string)
	{
		delete[] m_string;
		m_string = nullptr;
	}
}