#include "pch.h"
#include <Objbase.h>
#pragma comment(lib, "Ole32")
#pragma comment(lib, "Rpcrt4")
#include "TestCardReaderPlugin.h"


ProxyString CreateTransactionId()
{
	GUID transactionId;
	CoCreateGuid(&transactionId);

	unsigned char *pUID = NULL;
	UuidToString(&transactionId, &pUID);

	ProxyString str((char*)pUID);

	RpcStringFree(&pUID);

	return str;
}


DEFINE_PROXY_PLUGIN(TestCardReaderPlugin)


TestCardReaderPlugin::TestCardReaderPlugin()
{

}


void TestCardReaderPlugin::init(const ProxyStringMap &paramMap)
{
}


void TestCardReaderPlugin::exit()
{
}


void TestCardReaderPlugin::getStatus(ProxyStringMap &statusMap)
{
}


void TestCardReaderPlugin::sendPayment(const TransactionRequest &request, TransactionResponse &response)
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
		response.transactionId = CreateTransactionId();
		response.cardScheme = "VISA";
	}
}


void TestCardReaderPlugin::sendRefund(const TransactionRequest &request, TransactionResponse &response)
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
		response.transactionId = CreateTransactionId();
	}
}
