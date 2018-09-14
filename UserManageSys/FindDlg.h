#pragma once
#include "afxwin.h"

// CFindDlg 对话框

class CFindDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindDlg)

public:
	CFindDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindDlg();

// 对话框数据
	enum { IDD = IDD_FIND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void FindViewInit();
	CComboBox m_combo;
	CEdit m_edit;
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
