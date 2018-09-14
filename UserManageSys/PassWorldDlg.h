#pragma once
#include "afxwin.h"


// CPassWorldDlg �Ի���

class CPassWorldDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassWorldDlg)

public:
	CPassWorldDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassWorldDlg();

// �Ի�������
	enum { IDD = IDD_PASSWORLD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString account;//�˻�
	CString password;//����

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
