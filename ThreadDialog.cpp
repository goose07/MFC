// ThreadDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication1.h"
#include "ThreadDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// ThreadDialog 对话框

IMPLEMENT_DYNAMIC(ThreadDialog, CDialogEx)

ThreadDialog::ThreadDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ThreadDialog, pParent)
{
	m_ProcessID = 0;
}

ThreadDialog::~ThreadDialog()
{
}

void ThreadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TList, m_TList);
}


BEGIN_MESSAGE_MAP(ThreadDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_TList, &ThreadDialog::OnNMRClickTlist)
	ON_COMMAND(ID_CloseThread, &ThreadDialog::OnClosethread)
	ON_COMMAND(ID_Suspend, &ThreadDialog::OnSuspend)
	ON_COMMAND(ID_Restore, &ThreadDialog::OnRestore)
END_MESSAGE_MAP()


// ThreadDialog 消息处理程序


BOOL ThreadDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->m_TList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	this->m_TList.InsertColumn(0, L"线程ID", LVCFMT_LEFT, 60);
	this->m_TList.InsertColumn(1, L"进程ID", LVCFMT_LEFT, 60);
	this->m_TList.InsertColumn(2, L"线程优先级", LVCFMT_LEFT, 100);
	this->InitThreadData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void ThreadDialog::InitThreadData()
{
	this->m_TList.DeleteAllItems();
	HANDLE tRet = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	THREADENTRY32 t32 = { sizeof(THREADENTRY32) };
	int TList_Index = 0;
	CString TempBuff = 0;
	while (Thread32Next(tRet, &t32))
	{
		if (t32.th32OwnerProcessID == this->m_ProcessID)
		{
			TempBuff.Format(L"%d", t32.th32ThreadID);
			this->m_TList.InsertItem(TList_Index, TempBuff);

			TempBuff.Format(L"%d", t32.th32OwnerProcessID);
			this->m_TList.SetItemText(TList_Index, 1, TempBuff);

			TempBuff.Format(L"%d", t32.tpBasePri);
			this->m_TList.SetItemText(TList_Index, 2, TempBuff);
			TList_Index++;
		}
	}
}


void ThreadDialog::OnNMRClickTlist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT pt = { 0 };
	GetCursorPos(&pt);
	HMENU hMenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MENU2));
	HMENU hSubMenu = GetSubMenu(hMenu, 0);
	TrackPopupMenu(hSubMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	*pResult = 0;
}  


void ThreadDialog::OnClosethread()
{
	int number = (int)this->m_TList.GetFirstSelectedItemPosition();
	CString temp = m_TList.GetItemText(number-1 , 0);
	int ThreadId = _ttoi(temp);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, ThreadId);
	if (hThread != NULL)
	{
		DWORD ExitCode;
		GetExitCodeThread(hThread,&ExitCode);
		TerminateThread(hThread, ExitCode);
		CloseHandle(hThread);
	}
	this->InitThreadData();
}


void ThreadDialog::OnSuspend()
{
	int tempcount = (int)this->m_TList.GetFirstSelectedItemPosition();
	CString temp = this->m_TList.GetItemText(tempcount-1, 0);
	int ThreadId = _ttoi(temp);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, ThreadId);
	if (hThread != NULL)
	{
		SuspendThread(hThread);
		CloseHandle(hThread);
	}
}


void ThreadDialog::OnRestore()
{
	int tempcount = (int)this->m_TList.GetFirstSelectedItemPosition();
	CString temp = this->m_TList.GetItemText(tempcount-1, 0);
	int ThreadId = _ttoi(temp);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, ThreadId);
	if (hThread != NULL)
	{
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
}
