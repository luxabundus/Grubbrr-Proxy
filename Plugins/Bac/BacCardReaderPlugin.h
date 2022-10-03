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

	virtual void getStatus(ProxyStringMap &statusMap);

	virtual void sendPayment(const TransactionRequest &request, TransactionResponse &response);
	virtual void sendRefund(const TransactionRequest &request, TransactionResponse &response);

private:
	msclr::auto_gcroot<EMVStreamRequest^> m_pRequest;
};
