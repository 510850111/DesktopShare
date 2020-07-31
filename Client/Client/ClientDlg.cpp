
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_uPort(6969)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pClientSocket = NULL;
	m_lpszBmpData = NULL;

	m_SceenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_SceenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_bmpSize = 0;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT1, m_uPort);
	DDV_MinMaxUInt(pDX, m_uPort, 1, 65535);
	DDX_Control(pDX, IDC_IPADDRESS2, m_ipAddressCtrl);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CClientDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CClientDlg::OnBnClickedBtnConnect)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ipAddressCtrl.SetFocus();
	m_ipAddressCtrl.SetWindowText(L"127.0.0.1");

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CClientDlg::OnKillFocus()
{
	MessageBox(L"端口号的取值范围在: 1 至 65535之间。");
}

void CClientDlg::OnBnClickedBtnConnect()
{
	UpdateData(TRUE);
	
	CString strIpAddress;
	m_ipAddressCtrl.GetWindowText(strIpAddress);
	if (strIpAddress.IsEmpty() || strIpAddress == "0.0.0.0")
	{
		MessageBox(L"请输入合法的IPv4地址！");
	}

	if (m_uPort <= 0 || m_uPort > 65535)
	{
		MessageBox(L"端口号范围取值在:1至65535！");
	}
	

	//按钮不可用
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	//创建套接字
	pClientSocket = new CClientSocket;
	pClientSocket->SetPCClientMainDlg(this);
	if (FALSE == pClientSocket->Create())
	{
		CString strSocketCreatError;
		strSocketCreatError.Format(L"创建套接字失败！错误代号：%d", GetLastError());
		delete this->pClientSocket;
		pClientSocket = NULL;
		
		MessageBox(strSocketCreatError);
		//恢复按钮可用状态
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
		return;
	}

	//连接服务器
	
	if (FALSE == pClientSocket->Connect(strIpAddress, m_uPort))
	{
		CString strSocketCreatError;
		strSocketCreatError.Format(L"连接服务器失败！错误代号：%d", GetLastError());
		delete this->pClientSocket;
		pClientSocket = NULL;
		
		MessageBox(strSocketCreatError);
		//恢复按钮可用状态
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
		return;
	}

	CString str;
	GetWindowText(str);
	str.Format(L"【远程桌面客户端】已连接到 %s",strIpAddress);
	SetWindowText(str);
	GetDlgItem(IDC_IPADDRESS2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BTN_CONNECT, L"已连接");


	//恢复按钮可用状态
	//GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
}

void CClientDlg::SendBitMapStruct() 
{
	CaptSceen();
	pClientSocket->Send(&m_logBmp, sizeof(BITMAP), 0);
}

void CClientDlg::SendBitMapData() 
{
	if (m_lpszBmpData == NULL) return;
	
	int nCurrSendedData = 0;
	int nSendedBmpDataByte = 0;
	char *p = m_lpszBmpData;

	
	do
	{
		nCurrSendedData = pClientSocket->Send(p, m_bmpSize - nSendedBmpDataByte, 0);
		nSendedBmpDataByte += nCurrSendedData;
		p += nCurrSendedData;


	} while (nSendedBmpDataByte < m_bmpSize);
	
	//释放内存
	delete[] m_lpszBmpData;
	m_lpszBmpData = NULL;
	m_bmpSize = 0;

}

void CClientDlg::CaptSceen()
{
	CDC dc;
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	//创建兼容位图
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, m_SceenWidth, m_SceenHeight);

	//创建内存DC
	CDC tmpMemoryDC;
	tmpMemoryDC.CreateCompatibleDC(&dc);
	tmpMemoryDC.SelectObject(&bmp);
	tmpMemoryDC.BitBlt(0, 0, m_SceenWidth, m_SceenHeight, &dc, 0, 0, SRCCOPY);

	//获取位图结构信息
	//BITMAP logBmp;
	bmp.GetBitmap(&m_logBmp);

	//获取大小
	m_bmpSize = m_logBmp.bmWidthBytes * m_logBmp.bmHeight;

	//申请内存
	if (m_lpszBmpData != NULL) 
	{
		delete[] m_lpszBmpData;
		m_lpszBmpData = NULL;
	}

	m_lpszBmpData = new char[m_bmpSize];

	//设置位图头
	BITMAPINFOHEADER bih;
	bih.biBitCount = m_logBmp.bmBitsPixel;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = 0;
	bih.biHeight = m_logBmp.bmHeight;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = m_bmpSize;
	bih.biWidth = m_logBmp.bmWidth;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;

	GetDIBits(dc, bmp, 0, bih.biHeight, m_lpszBmpData, (BITMAPINFO *)&bih, DIB_RGB_COLORS);

	//清理
	dc.DeleteDC();
	tmpMemoryDC.DeleteDC();
	bmp.DeleteObject();
}

void CClientDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
