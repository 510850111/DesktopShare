#include "stdafx.h"
#include "ListenSocket.h"
#include "ServerDlg.h"


CListenSocket::CListenSocket(CServerDlg *p_Dlg)
{
	pMainDlg = p_Dlg;
}


CListenSocket::~CListenSocket()
{
}


void CListenSocket::OnAccept(int nErrorCode)
{	
	pMainDlg->OnAccept();

	CSocket::OnAccept(nErrorCode);

}
