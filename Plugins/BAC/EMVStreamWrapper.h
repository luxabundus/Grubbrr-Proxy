#pragma once
#include <vcclr.h>
#include <msclr\auto_gcroot.h>
#include <Common/Includes.h>

#using "CSP.EMV.InteropStream.dll"

#pragma warning (disable: 4691)

using namespace System;
using namespace System::Reflection;
using namespace CSP::EMV::InteropStream;


ref class EMVStreamRequestWrapper
{
public:
	ProxyString sendPayment(ProxyStringMap &params);

private:
	EMVStreamRequest m_request;
};

