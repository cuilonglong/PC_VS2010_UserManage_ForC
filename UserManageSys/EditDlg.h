#pragma once

#include "PassWorldDlg.h"
#include "afxcmn.h"

// CEditDlg 对话框

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditDlg();

// 对话框数据
	enum { IDD = IDD_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ListAddLine(int line);
	void ListAddRow(int row,int ID,CString IP,int port,CString Time);
	CListCtrl m_edit_list;
	int IDNUM;
	afx_msg void OnNMRClickEditList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On1();
	afx_msg void On2();
	afx_msg void On3();
	afx_msg void On4();
	afx_msg void On5();
	afx_msg void On6();

	void ShowEditListView();//初始化链表控件
	afx_msg void On7();
	afx_msg void OnNMDblclkEditList(NMHDR *pNMHDR, LRESULT *pResult);
};
