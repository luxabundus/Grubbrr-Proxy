#include "EMVStreamWrapper.h"
#include <msclr/marshal.h>


ProxyString EMVStreamRequestWrapper::sendPayment(ProxyStringMap &params)
{
	msclr::interop::marshal_context ctx;

	m_request.transactionType = "SALE";
	m_request.totalAmount = gcnew String(params["subTotal"]);
	m_request.terminalId = gcnew String(params["terminalId"]);
	m_request.invoice = gcnew String(params["orderId"]);

	return ctx.marshal_as<const char*>(m_request.sendData());
}

