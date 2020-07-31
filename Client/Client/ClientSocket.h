#pragma once
#include "afxsock.h"
class CClientDlg;
class CClientSocket :
	public CSocket
{
protected:
	CClientDlg *m_pCClientMainDlg;

public:
	CClientSocket();
	CClientSocket(CClientDlg *pCClientDlg);
	~CClientSocket();
	virtual void OnAccept(int nErrorCode);
	void SetPCClientMainDlg(CClientDlg *pCClientDlg);
	CClientDlg * GetCClientMainDlg();
	virtual void OnReceive(int nErrorCode);
};

