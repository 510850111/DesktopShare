#pragma once

// CShowDlg 对话框
class CClientSocket;
class CShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	CShowDlg(CClientSocket *pClientSocket, CWnd* pParent = NULL);
	CClientSocket *m_pClientSocket;
	UINT m_timerInterval;
	UINT m_nBmpSize;
	char *m_lpszBmpData;
	BITMAP m_logBmp;//	位图结构
	virtual ~CShowDlg();
	void onGetClientSceen();
	int m_getClientSceenIdTimer;
	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWCLIENT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};
