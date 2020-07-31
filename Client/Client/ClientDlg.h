
// ClientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ClientSocket.h"


// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	BITMAP m_logBmp; //保存结构信息
	char* m_lpszBmpData; //位图数据指针
	UINT m_bmpSize;		//位图大小
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	void SendBitMapStruct();//发送位图结构
	void SendBitMapData();//发送位图数据
	void CaptSceen();
};
