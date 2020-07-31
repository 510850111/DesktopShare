
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ClientSocket.h"


// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnKillFocus();
	CIPAddressCtrl m_ipAddressCtrl;
	UINT m_uPort;
	CClientSocket * pClientSocket;
	UINT m_SceenWidth;
	UINT m_SceenHeight;
	BITMAP m_logBmp; //����ṹ��Ϣ
	char* m_lpszBmpData; //λͼ����ָ��
	UINT m_bmpSize;		//λͼ��С
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	void SendBitMapStruct();//����λͼ�ṹ
	void SendBitMapData();//����λͼ����
	void CaptSceen();
};
