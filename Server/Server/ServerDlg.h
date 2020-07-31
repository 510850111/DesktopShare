
// ServerDlg.h : ͷ�ļ�
//

#pragma once
#include "ListenSocket.h"
#include "afxcmn.h"

// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	CListenSocket *m_pListenSocket;
	CListCtrl m_list;
	void OnAccept();
	//int curr_item_dbclicked;

	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
