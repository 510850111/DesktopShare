
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg �Ի���



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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ipAddressCtrl.SetFocus();
	m_ipAddressCtrl.SetWindowText(L"127.0.0.1");

	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CClientDlg::OnKillFocus()
{
	MessageBox(L"�˿ںŵ�ȡֵ��Χ��: 1 �� 65535֮�䡣");
}

void CClientDlg::OnBnClickedBtnConnect()
{
	UpdateData(TRUE);
	
	CString strIpAddress;
	m_ipAddressCtrl.GetWindowText(strIpAddress);
	if (strIpAddress.IsEmpty() || strIpAddress == "0.0.0.0")
	{
		MessageBox(L"������Ϸ���IPv4��ַ��");
	}

	if (m_uPort <= 0 || m_uPort > 65535)
	{
		MessageBox(L"�˿ںŷ�Χȡֵ��:1��65535��");
	}
	

	//��ť������
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	//�����׽���
	pClientSocket = new CClientSocket;
	pClientSocket->SetPCClientMainDlg(this);
	if (FALSE == pClientSocket->Create())
	{
		CString strSocketCreatError;
		strSocketCreatError.Format(L"�����׽���ʧ�ܣ�������ţ�%d", GetLastError());
		delete this->pClientSocket;
		pClientSocket = NULL;
		
		MessageBox(strSocketCreatError);
		//�ָ���ť����״̬
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
		return;
	}

	//���ӷ�����
	
	if (FALSE == pClientSocket->Connect(strIpAddress, m_uPort))
	{
		CString strSocketCreatError;
		strSocketCreatError.Format(L"���ӷ�����ʧ�ܣ�������ţ�%d", GetLastError());
		delete this->pClientSocket;
		pClientSocket = NULL;
		
		MessageBox(strSocketCreatError);
		//�ָ���ť����״̬
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
		return;
	}

	CString str;
	GetWindowText(str);
	str.Format(L"��Զ������ͻ��ˡ������ӵ� %s",strIpAddress);
	SetWindowText(str);
	GetDlgItem(IDC_IPADDRESS2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BTN_CONNECT, L"������");


	//�ָ���ť����״̬
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
	
	//�ͷ��ڴ�
	delete[] m_lpszBmpData;
	m_lpszBmpData = NULL;
	m_bmpSize = 0;

}

void CClientDlg::CaptSceen()
{
	CDC dc;
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	//��������λͼ
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, m_SceenWidth, m_SceenHeight);

	//�����ڴ�DC
	CDC tmpMemoryDC;
	tmpMemoryDC.CreateCompatibleDC(&dc);
	tmpMemoryDC.SelectObject(&bmp);
	tmpMemoryDC.BitBlt(0, 0, m_SceenWidth, m_SceenHeight, &dc, 0, 0, SRCCOPY);

	//��ȡλͼ�ṹ��Ϣ
	//BITMAP logBmp;
	bmp.GetBitmap(&m_logBmp);

	//��ȡ��С
	m_bmpSize = m_logBmp.bmWidthBytes * m_logBmp.bmHeight;

	//�����ڴ�
	if (m_lpszBmpData != NULL) 
	{
		delete[] m_lpszBmpData;
		m_lpszBmpData = NULL;
	}

	m_lpszBmpData = new char[m_bmpSize];

	//����λͼͷ
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

	//����
	dc.DeleteDC();
	tmpMemoryDC.DeleteDC();
	bmp.DeleteObject();
}

void CClientDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
