#pragma once


class ProxyRegKey : public RegKey
{
public:
	static const String ROOT_KEY;
	static const String TERMINAL_ROOT_KEY;
	static const String PLUGIN_ROOT_KEY;

	void loadSettings(ProxyStringMap &settings)
	{
		forEachStringValue(
			[&settings](const char *name, const char *value) mutable
			{
				if (*name && (settings.find(name) == settings.end()))
				{
					settings[name] = value;
				}
				return true;
			}
		);
	}
};

