#pragma once

// CShowDlg �Ի���
class CClientSocket;
class CShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CShowDlg(CClientSocket *pClientSocket, CWnd* pParent = NULL);
	CClientSocket *m_pClientSocket;
	UINT m_timerInterval;
	UINT m_nBmpSize;
	char *m_lpszBmpData;
	BITMAP m_logBmp;//	λͼ�ṹ
	virtual ~CShowDlg();
	void onGetClientSceen();
	int m_getClientSceenIdTimer;
	

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWCLIENT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};
