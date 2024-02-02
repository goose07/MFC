#pragma once


// ThreadDialog 对话框

class ThreadDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ThreadDialog)

public:
	ThreadDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ThreadDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ThreadDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_ProcessID;
	virtual BOOL OnInitDialog();
	CListCtrl m_TList;
	void InitThreadData();
	afx_msg void OnNMRClickTlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClosethread();
	afx_msg void OnSuspend();
	afx_msg void OnRestore();
};
