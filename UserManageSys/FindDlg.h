#pragma once
#include "afxwin.h"

// CFindDlg �Ի���

class CFindDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindDlg)

public:
	CFindDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindDlg();

// �Ի�������
	enum { IDD = IDD_FIND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void FindViewInit();
	CComboBox m_combo;
	CEdit m_edit;
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
