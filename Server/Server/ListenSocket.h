#pragma once
#include "afxsock.h"

class CServerDlg;
class CListenSocket : public CSocket
{
private:
	CServerDlg *pMainDlg;
public:
	virtual void OnAccept(int nErrorCode);
	CListenSocket(CServerDlg *p_Dlg);
	~CListenSocket();
};

