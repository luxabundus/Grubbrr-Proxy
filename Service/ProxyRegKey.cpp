#include "pch.h"
#include "ProxyRegKey.h"


// #if defined(_WIN64)
// const String ProxyRegKey::ROOT_KEY = "Software\\Wow6432Node\\Grubbrr\\Proxy";
// #else
const String ProxyRegKey::ROOT_KEY = "Software\\Grubbrr\\Proxy";
//#endif
const String ProxyRegKey::TERMINAL_ROOT_KEY = ProxyRegKey::ROOT_KEY + "\\Terminals";
const String ProxyRegKey::PLUGIN_ROOT_KEY = ProxyRegKey::ROOT_KEY + "\\Plugins";
