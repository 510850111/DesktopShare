#include "stdafx.h"
#include "ClientSocket.h"
#include "ClientDlg.h"
#include "afxdialogex.h"


CClientSocket::CClientSocket()
{
	m_pCClientMainDlg = NULL;
}

CClientSocket::CClientSocket(CClientDlg *pCClientDlg)
{
	m_pCClientMainDlg = pCClientDlg;
}

CClientSocket::~CClientSocket()
{
	if (m_pCClientMainDlg != NULL)
	{
		m_pCClientMainDlg = NULL;
	}
}



void CClientSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	

	CSocket::OnAccept(nErrorCode);
}


void CClientSocket::SetPCClientMainDlg(CClientDlg *pCClientDlg)
{
	m_pCClientMainDlg = pCClientDlg;
}

CClientDlg * CClientSocket::GetCClientMainDlg()
{
	return m_pCClientMainDlg;
}

void CClientSocket::OnReceive(int nErrorCode)
{
	int nMsgType;
	Receive(&nMsgType, sizeof(nMsgType), 0);

	//CString strMsgType;
	//strMsgType.Format(L"strMsgType = %d", nMsgType);
	//m_pCClientMainDlg->MessageBox(strMsgType);

	switch (nMsgType)
	{
	case BITMAP_STRUCT:
		m_pCClientMainDlg->SendBitMapStruct();
		break;

	case BITMAP_DATA:
		m_pCClientMainDlg->SendBitMapData();
		break;

	default:
		break;
	}

	CSocket::OnReceive(nErrorCode);
}
