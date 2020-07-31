// ShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ShowDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"


// CShowDlg �Ի���

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


// CShowDlg ��Ϣ�������


BOOL CShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strAddress, windowTitle;
	UINT port;
	m_pClientSocket->GetPeerName(strAddress, port);
	windowTitle.Format(L"���ڿ��� %s:%d", strAddress, port);
	SetWindowText(windowTitle);

	SetTimer(m_getClientSceenIdTimer, m_timerInterval, NULL);
	//SetTimer(m_getClientSceenIdTimer, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CShowDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	//�Զ���Э��
	int nMsgType = BITMAP_STRUCT;
	//���͸��ͻ���λͼ�ṹ����
	int nRet = m_pClientSocket->Send(&nMsgType, sizeof(nMsgType), 0);
	if (nRet <=0 )
	{
		KillTimer(GET_CLIENT_SCEEN_ID_TIMER);
		MessageBox(L"�ͻ���ͨ��ʧ�ܡ�");
		EndDialog(-1);
		return;
	}

	//���տͻ��˷��ص�λͼ�ṹ��Ϣ
	nRet = m_pClientSocket->Receive(&m_logBmp, sizeof(BITMAP), 0);
	if (nRet != sizeof(BITMAP))
	{
		KillTimer(GET_CLIENT_SCEEN_ID_TIMER);
		MessageBox(L"����λͼ��Ϣ�ṹ����ȷ��");
		EndDialog(-1);
		return;
	}

	//����λͼ������Ϣ����
	nMsgType = BITMAP_DATA;
	nRet = m_pClientSocket->Send(&nMsgType, sizeof(nMsgType), 0);
	if (nRet <= 0) 
	{ 
		KillTimer(GET_CLIENT_SCEEN_ID_TIMER);
		MessageBox(L"�ͻ���ͨ��ʧ�ܡ�");
		EndDialog(-1);
		return; 
	}

	//λͼ��С
	int nBitmapSize = m_logBmp.bmWidthBytes * m_logBmp.bmHeight;
	m_nBmpSize = nBitmapSize;
	if (m_lpszBmpData != NULL)
	{
		delete [] m_lpszBmpData;
		m_lpszBmpData = NULL;
	}
	m_lpszBmpData = new char[m_nBmpSize];

	//������Ϣ
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
		MessageBox(_T("λͼ����ʧ��"), _T("��ʾ"), MB_OK);
		EndDialog(-1);
		return;
	}

	//ˢ�½���
	Invalidate(FALSE);
}

void CShowDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	if (m_lpszBmpData == NULL)
	{
		return;
	}
	
	//λͼͷ
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



	//����λͼ�ṹ
	CBitmap bmp;
	if (FALSE == bmp.CreateBitmapIndirect(&m_logBmp))
		return;

	//λͼû�гɹ�
	if (bmp.m_hObject == NULL)
		return;

	//�����ڴ�DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	SetDIBits(memDC.m_hDC, bmp, 0, m_logBmp.bmHeight, m_lpszBmpData, (BITMAPINFO *)&bih, DIB_RGB_COLORS);

	//��ȡ�ͻ�����С
	CRect rect;
	GetClientRect(&rect);
	memDC.SelectObject(&bmp);

	//������ͼ
	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(0, 0, rect.Width(), rect.Height(),
		&memDC, 0, 0, m_logBmp.bmWidth, m_logBmp.bmHeight, SRCCOPY);


	//�ͷ��ڴ�
	delete[] m_lpszBmpData;
	m_lpszBmpData = NULL;

}
