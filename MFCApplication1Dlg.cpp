
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <psapi.h>
#include "ThreadDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ProcessDialog, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLIST, m_pList);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_PLIST, &CMFCApplication1Dlg::OnRclickPlist)
	ON_COMMAND(ID_CloseProcess, &CMFCApplication1Dlg::OnCloseprocess)
	ON_COMMAND(ID_OpenThread, &CMFCApplication1Dlg::OnOpenthread)
	ON_COMMAND(ID_Refresh, &CMFCApplication1Dlg::OnRefresh)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->m_pList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	this->m_pList.InsertColumn(0, L"进程名", LVCFMT_LEFT, 150);
	this->m_pList.InsertColumn(1, L"父进程ID", LVCFMT_LEFT, 80);
	this->m_pList.InsertColumn(2, L"进程ID", LVCFMT_LEFT, 65);
	this->m_pList.InsertColumn(3, L"线程数", LVCFMT_LEFT, 65);
	this->m_pList.InsertColumn(4, L"进程优先级", LVCFMT_LEFT, 90);
	this->m_pList.InsertColumn(5, L"进程路径", LVCFMT_LEFT, 600);
	this->InitListData();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
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
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnRclickPlist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT pt = { 0 };
	GetCursorPos(&pt);
	HMENU hMenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MENU1));
	HMENU hSubMenu = GetSubMenu(hMenu, 0);
	TrackPopupMenu(hSubMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	*pResult = 0;
}


void CMFCApplication1Dlg::OnCloseprocess()
{
	int  number = (int)this->m_pList.GetFirstSelectedItemPosition();
	CString TempNumber;
	TempNumber = this->m_pList.GetItemText(number - 1, 2);
	int pidRet = _ttoi(TempNumber);
	HANDLE pRet = OpenProcess(PROCESS_ALL_ACCESS, NULL, pidRet);
	TerminateProcess(pRet, 0);
	CloseHandle(pRet);
	this->InitListData();
}


void CMFCApplication1Dlg::OnOpenthread()
{
	int  number = (int)this->m_pList.GetFirstSelectedItemPosition();
	CString TempNumber;
	TempNumber = this->m_pList.GetItemText(number - 1, 2);
	ThreadDialog TDialogObj;
	TDialogObj.m_ProcessID = _ttoi(TempNumber);
	TDialogObj.DoModal();
}


void CMFCApplication1Dlg::OnRefresh()
{
	this->InitListData();
}

void CMFCApplication1Dlg::InitListData()
{
	this->m_pList.DeleteAllItems();
	HANDLE pRet = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 p32 = { sizeof(PROCESSENTRY32) };
	Process32First(pRet, &p32);
	int list_Index = 0;
	CString TempBuff = 0;
	do
	{
		TempBuff.Format(L"%d ", list_Index);
		this->m_pList.InsertItem(list_Index, TempBuff + p32.szExeFile);

		TempBuff.Format(L"%d", p32.th32ParentProcessID);
		this->m_pList.SetItemText(list_Index, 1, TempBuff);

		TempBuff.Format(L"%d", p32.th32ProcessID);
		this->m_pList.SetItemText(list_Index, 2, TempBuff);

		TempBuff.Format(L"%d", p32.cntThreads);
		this->m_pList.SetItemText(list_Index, 3, TempBuff);

		TempBuff.Format(L"%d", p32.pcPriClassBase);
		this->m_pList.SetItemText(list_Index, 4, TempBuff);

		HANDLE opRet = OpenProcess(PROCESS_ALL_ACCESS, NULL, p32.th32ProcessID);
		WCHAR NameBuff[MAX_PATH] = { 0 };
		if (!opRet)
		{
			this->m_pList.SetItemText(list_Index, 5, L"系统进程禁止访问哦！");
		}
		else
		{
			GetModuleFileNameEx(opRet, NULL, NameBuff, MAX_PATH);
			this->m_pList.SetItemText(list_Index, 5, NameBuff);
			CloseHandle(opRet);
		}
		list_Index++;
	} while (Process32Next(pRet, &p32));
}
