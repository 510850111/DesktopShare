
// ServerDlg.cpp : 实现文件
//
#include <CString>
#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"
#include "ShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SERVER_PORT 6969

// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pListenSocket = NULL;
	
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CServerDlg::OnBnClickedBtnStart)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CServerDlg::OnDblclkList)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	/*套接字的创建*/
	m_pListenSocket = new CListenSocket(this);
	bool isCreatListenSocketSuccess = m_pListenSocket->Create(SERVER_PORT,SOCK_STREAM);
	if (isCreatListenSocketSuccess == FALSE)
	{
		CString str;
		str.Format(L"套接字创建失败，错误代码：%d", GetLastError());
		delete m_pListenSocket;
		MessageBox(str);
		EndDialog(IDOK);//关闭对话框
		return TRUE;
	}
	/*监听*/
	bool isListenSuccess = m_pListenSocket->Listen();
	if (isListenSuccess == FALSE)
	{
		CString str;
		str.Format(L"启动监听失败，错误代码：%d", GetLastError());
		delete m_pListenSocket;
		MessageBox(str);
		EndDialog(IDOK);//关闭对话框
		return TRUE;
	}

	m_list.InsertColumn(0, L"IPv4地址", LVCFMT_LEFT, 200);
	m_list.InsertColumn(1, L"端口号", LVCFMT_LEFT, 200);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnAccept()
{
	//MessageBox(L"收到连接");
	CClientSocket *pClientSocket = new CClientSocket();
	m_pListenSocket->Accept(*pClientSocket);

	//ip and port
	CString strIpAddress, strPort;
	UINT port;
		
	pClientSocket->GetPeerName(strIpAddress, port);

	int count = m_list.GetItemCount();
	m_list.InsertItem(count, strIpAddress);
	strPort.Format(L"%d", port);
	m_list.SetItemText(count, 1, strPort);
	m_list.SetItemData(count, (DWORD)pClientSocket);
}



/*
开启服务器
*/
void CServerDlg::OnBnClickedBtnStart()
{
	m_pListenSocket = new CListenSocket(this);
}


void CServerDlg::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(L"this is double click!");

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nSel = m_list.GetNextSelectedItem(pos);
	if (nSel < 0)
	{
		return;
	}
	CClientSocket *pClientSocket = (CClientSocket *)m_list.GetItemData(nSel);
	/*
	CString strCurrAdress = m_list.GetItemText(nSel, 0);
	CString strCurrPort = m_list.GetItemText(nSel, 1);
	CString strWindowName = strCurrAdress +":"+ strCurrPort;
	*/
	CShowDlg *pShowDlg = new CShowDlg(pClientSocket);
	if (FALSE == pShowDlg->Create(IDD_SHOWCLIENT_DLG))
	{
		delete pShowDlg;
		pShowDlg = NULL;
	}

	pShowDlg->ShowWindow(SW_SHOW);



	*pResult = 0;
}
