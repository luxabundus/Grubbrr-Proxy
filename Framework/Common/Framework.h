#pragma once


#include <string>
#include <map>
#include <exception>

#include <windows.h>


#if defined(GRUBBRR_PROXY_COMMON_DLL)
#define GRUBBRR_PROXY_COMMON_DECLSPEC __declspec(dllexport)
#else 
#define GRUBBRR_PROXY_COMMON_DECLSPEC __declspec(dllimport)
#pragma comment(lib, "Grubbrr.Proxy.Common.lib")
#endif
