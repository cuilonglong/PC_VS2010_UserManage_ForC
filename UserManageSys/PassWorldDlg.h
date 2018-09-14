#pragma once
#include "afxwin.h"


// CPassWorldDlg 对话框

class CPassWorldDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassWorldDlg)

public:
	CPassWorldDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPassWorldDlg();

// 对话框数据
	enum { IDD = IDD_PASSWORLD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString account;//账户
	CString password;//密码

	afx_msg void OnStnClickedPasswordStatic();
	virtual BOOL OnInitDialog();
	CStatic m_passworld_static;
	CStatic m_passworld_static1;
	CFont m_login_Font; 

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
