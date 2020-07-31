
// ServerDlg.cpp : ʵ���ļ�
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

// CServerDlg �Ի���



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


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	/*�׽��ֵĴ���*/
	m_pListenSocket = new CListenSocket(this);
	bool isCreatListenSocketSuccess = m_pListenSocket->Create(SERVER_PORT,SOCK_STREAM);
	if (isCreatListenSocketSuccess == FALSE)
	{
		CString str;
		str.Format(L"�׽��ִ���ʧ�ܣ�������룺%d", GetLastError());
		delete m_pListenSocket;
		MessageBox(str);
		EndDialog(IDOK);//�رնԻ���
		return TRUE;
	}
	/*����*/
	bool isListenSuccess = m_pListenSocket->Listen();
	if (isListenSuccess == FALSE)
	{
		CString str;
		str.Format(L"��������ʧ�ܣ�������룺%d", GetLastError());
		delete m_pListenSocket;
		MessageBox(str);
		EndDialog(IDOK);//�رնԻ���
		return TRUE;
	}

	m_list.InsertColumn(0, L"IPv4��ַ", LVCFMT_LEFT, 200);
	m_list.InsertColumn(1, L"�˿ں�", LVCFMT_LEFT, 200);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnAccept()
{
	//MessageBox(L"�յ�����");
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
����������
*/
void CServerDlg::OnBnClickedBtnStart()
{
	m_pListenSocket = new CListenSocket(this);
}


void CServerDlg::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
