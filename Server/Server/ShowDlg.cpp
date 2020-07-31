// ShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ShowDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"


// CShowDlg 对话框

IMPLEMENT_DYNAMIC(CShowDlg, CDialogEx)

CShowDlg::CShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOWCLIENT_DLG, pParent)
{
	m_pClientSocket = NULL;
	m_lpszBmpData = NULL;
	m_getClientSceenIdTimer = GET_CLIENT_SCEEN_ID_TIMER;
	m_timerInterval = (int)(1000/24);
	m_nBmpSize = 0;
}

CShowDlg::CShowDlg(CClientSocket *pClientSocket, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOWCLIENT_DLG, pParent)
{
	m_pClientSocket = pClientSocket;
	m_lpszBmpData = NULL;
	m_getClientSceenIdTimer = GET_CLIENT_SCEEN_ID_TIMER;
	m_timerInterval = (int)(1000 / 24);
	m_nBmpSize = 0;
}

CShowDlg::~CShowDlg()
{

}

void CShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CShowDlg 消息处理程序


BOOL CShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strAddress, windowTitle;
	UINT port;
	m_pClientSocket->GetPeerName(strAddress, port);
	windowTitle.Format(L"正在控制 %s:%d", strAddress, port);
	SetWindowText(windowTitle);

	SetTimer(m_getClientSceenIdTimer, m_timerInterval, NULL);
	//SetTimer(m_getClientSceenIdTimer, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CShowDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//MessageBox(L"OnTimer");
	//CString strnIDEvent;
	//strnIDEvent.Format(L"nIDEvent = %d", nIDEvent);
	//MessageBox(strnIDEvent);

	switch (nIDEvent)
	{
	case GET_CLIENT_SCEEN_ID_TIMER:
		onGetClientSceen();
		break;
	}

	//CDialogEx::OnTimer(nIDEvent);
}


void CShowDlg::onGetClientSceen()
{
	//自定义协议
	int nMsgType = BITMAP_STRUCT;
	//发送给客户端位图结构请求
	int nRet = m_pClientSocket->Send(&nMsgType, sizeof(nMsgType), 0);
	if (nRet <=0 )
	{
		KillTimer(GET_CLIENT_SCEEN_ID_TIMER);
		MessageBox(L"客户端通信失败。");
		EndDialog(-1);
		return;
	}

	//接收客户端返回的位图结构消息
	nRet = m_pClientSocket->Receive(&m_logBmp, sizeof(BITMAP), 0);
	if (nRet != sizeof(BITMAP))
	{
		KillTimer(GET_CLIENT_SCEEN_ID_TIMER);
		MessageBox(L"接收位图消息结构不正确。");
		EndDialog(-1);
		return;
	}

	//发送位图数据消息请求
	nMsgType = BITMAP_DATA;
	nRet = m_pClientSocket->Send(&nMsgType, sizeof(nMsgType), 0);
	if (nRet <= 0) 
	{ 
		KillTimer(GET_CLIENT_SCEEN_ID_TIMER);
		MessageBox(L"客户端通信失败。");
		EndDialog(-1);
		return; 
	}

	//位图大小
	int nBitmapSize = m_logBmp.bmWidthBytes * m_logBmp.bmHeight;
	m_nBmpSize = nBitmapSize;
	if (m_lpszBmpData != NULL)
	{
		delete [] m_lpszBmpData;
		m_lpszBmpData = NULL;
	}
	m_lpszBmpData = new char[m_nBmpSize];

	//接收消息
	int nCurrRecvData = 0;
	int nRecvBmpDataByteTotal = 0;
	char *p = m_lpszBmpData;

	do
	{
		nCurrRecvData = m_pClientSocket->Receive(p, (nBitmapSize - nRecvBmpDataByteTotal));
		if (nCurrRecvData <= 0)
		{
			delete[] m_lpszBmpData;
			m_lpszBmpData = NULL;
			return;
		}
		nRecvBmpDataByteTotal += nCurrRecvData;
		p += nCurrRecvData;
	} while (nRecvBmpDataByteTotal < m_nBmpSize);

	if (nRecvBmpDataByteTotal != m_nBmpSize)
	{
		KillTimer(GET_CLIENT_SCEEN_ID_TIMER);
		MessageBox(_T("位图接收失败"), _T("提示"), MB_OK);
		EndDialog(-1);
		return;
	}

	//刷新界面
	Invalidate(FALSE);
}

void CShowDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	if (m_lpszBmpData == NULL)
	{
		return;
	}
	
	//位图头
	BITMAPINFOHEADER bih;
	bih.biBitCount = m_logBmp.bmBitsPixel;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = 0;
	bih.biHeight = m_logBmp.bmHeight;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = m_logBmp.bmWidthBytes * m_logBmp.bmHeight;
	bih.biWidth = m_logBmp.bmWidth;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;



	//构造位图结构
	CBitmap bmp;
	if (FALSE == bmp.CreateBitmapIndirect(&m_logBmp))
		return;

	//位图没有成功
	if (bmp.m_hObject == NULL)
		return;

	//创建内存DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	SetDIBits(memDC.m_hDC, bmp, 0, m_logBmp.bmHeight, m_lpszBmpData, (BITMAPINFO *)&bih, DIB_RGB_COLORS);

	//获取客户区大小
	CRect rect;
	GetClientRect(&rect);
	memDC.SelectObject(&bmp);

	//拉伸贴图
	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(0, 0, rect.Width(), rect.Height(),
		&memDC, 0, 0, m_logBmp.bmWidth, m_logBmp.bmHeight, SRCCOPY);


	//释放内存
	delete[] m_lpszBmpData;
	m_lpszBmpData = NULL;

}
