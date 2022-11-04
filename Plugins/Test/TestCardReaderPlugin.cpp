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
	UuidToStringA(&transactionId, &pUID);

	ProxyString str((char*)pUID);

	RpcStringFreeA(&pUID);

	return str;
}


DEFINE_PROXY_PLUGIN(TestCardReaderPlugin)


TestCardReaderPlugin::TestCardReaderPlugin()
{

}


void TestCardReaderPlugin::init(const ProxyStringMap &params)
{
}


void TestCardReaderPlugin::exit()
{
}


void TestCardReaderPlugin::queryStatus(Transaction &transactions)
{
	transactions.resultData["status"] = "online";
}


void TestCardReaderPlugin::sendPayment(Transaction &transaction)
{
	if (transaction.requestData["orderId"] == "111")
	{
		transaction.status = "rejected";
		transaction.statusCode = "11";
		transaction.statusMessage = "payment: expired";
	}
	else if (transaction.requestData["orderId"] == "222")
	{
		transaction.status = "error";
		transaction.statusCode = "99";
		transaction.statusMessage = "payment: connection error";
	}
	else
	{
		transaction.status = "success";
		transaction.statusCode = "00";
		transaction.statusMessage = "payment: success";
		transaction.resultData["transactionId"] = CreateTransactionId();
		transaction.resultData["cardScheme"] = "VISA";
	}
}


void TestCardReaderPlugin::sendRefund(Transaction &transaction)
{
	if (transaction.requestData["orderId"] == "111")
	{
		transaction.status = "rejected";
		transaction.statusCode = "88";
		transaction.statusMessage = "refund: unapproved";
	}
	else if (transaction.requestData["orderId"] == "222")
	{
		transaction.status = "error";
		transaction.statusCode = "99";
		transaction.statusMessage = "refund: connection error";
	}
	else
	{
		transaction.status = "success";
		transaction.statusCode = "00";
		transaction.statusMessage = "refund: success";
		transaction.resultData["transactionId"] = CreateTransactionId();
	}
}


void TestCardReaderPlugin::sendVoid(Transaction &transaction)
{
	if (transaction.requestData["orderId"] == "111")
	{
		transaction.status = "rejected";
		transaction.statusCode = "88";
		transaction.statusMessage = "void: unapproved";
	}
	else if (transaction.requestData["orderId"] == "222")
	{
		transaction.status = "error";
		transaction.statusCode = "99";
		transaction.statusMessage = "void: connection error";
	}
	else
	{
		transaction.status = "success";
		transaction.statusCode = "00";
		transaction.statusMessage = "void: success";
		transaction.resultData["transactionId"] = CreateTransactionId();
	}
}


void TestCardReaderPlugin::sendSettlement(Transaction &transaction)
{
	transaction.status = "success";
	transaction.statusCode = "00";
	transaction.statusMessage = "settle: success";
}
