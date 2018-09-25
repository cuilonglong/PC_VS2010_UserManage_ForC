#pragma once
#include "afxwin.h"
#include "afxcmn.h"

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
	void ComboViewInit();
	void ListAddLine(int line);
	void ListAddRow(int row,int ID,int port,int status,CString IP,CString QQ);

	CComboBox m_combo;
	CEdit m_edit;
	int IDNUM;
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_find_list;
	afx_msg void OnNMDblclkFindEditList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeFindCombo();
	afx_msg void OnNMRClickFindEditList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On8();
};
