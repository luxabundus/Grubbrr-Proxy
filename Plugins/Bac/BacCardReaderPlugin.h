#pragma once
#include <vcclr.h>
#include <msclr\auto_gcroot.h>
#include <Common/Includes.h>

#using "CSP.EMV.InteropStream.dll"

#pragma warning (disable: 4691)

using namespace System;
using namespace System::Reflection;
using namespace CSP::EMV::InteropStream;


class BacCardReaderPlugin : public ProxyCardReaderPlugin
{
public:
	BacCardReaderPlugin();

	virtual void init(const ProxyStringMap &paramMap);
	virtual void exit();

	virtual ProxyStringMap sendPayment(ProxyStringMap &request);
	virtual ProxyStringMap sendRefund(ProxyStringMap &request);
	virtual ProxyStringMap settleTransactions(ProxyStringMap &settings);
	virtual ProxyStringMap queryStatus();

private:
	msclr::auto_gcroot<EMVStreamRequest^> m_pRequest;
};
