#include <vcclr.h>
#include "BacCardReaderPlugin.h"


using namespace System;
using namespace System::Reflection;


DEFINE_PROXY_PLUGIN(BacCardReaderPlugin)


BacCardReaderPlugin::BacCardReaderPlugin() :
	m_pWrapper(nullptr)
{
}


void BacCardReaderPlugin::init(const ProxyStringMap &paramMap)
{
	//	Assembly^ assembly = Assembly::Load("CSP.EMV.InteropStream.dll");
	// 	Type^ type = assembly->GetType("EMVStreamRequest");
	// 
	// 	ConstructorInfo^ constructor = type->GetConstructor(Type::EmptyTypes);
	// 	Object^ instance = constructor->Invoke(gcnew array<Object^>(0));
}


void BacCardReaderPlugin::exit()
{
}


void BacCardReaderPlugin::getStatus(ProxyStringMap &statusMap)
{
}


void BacCardReaderPlugin::sendPayment(const TransactionRequest &request, TransactionResponse &response)
{
	if (request.orderId == "111")
	{
		response.status = "rejected";
		response.statusCode = "11";
		response.statusMessage = "payment: expired";
	}
	else if (request.orderId == "222")
	{
		response.status = "error";
		response.statusCode = "99";
		response.statusMessage = "payment: connection error";
	}
	else
	{
		response.status = "success";
		response.statusCode = "00";
		response.statusMessage = "payment: success";
		response.transactionId = "ABC-123";
		response.cardScheme = "VISA";
	}
}


void BacCardReaderPlugin::sendRefund(const TransactionRequest &request, TransactionResponse &response)
{
	if (request.orderId == "111")
	{
		response.status = "rejected";
		response.statusCode = "88";
		response.statusMessage = "refund: unapproved";
	}
	else if (request.orderId == "222")
	{
		response.status = "error";
		response.statusCode = "99";
		response.statusMessage = "refund: connection error";
	}
	else
	{
		response.status = "success";
		response.statusCode = "00";
		response.statusMessage = "refund: success";
	}
}
