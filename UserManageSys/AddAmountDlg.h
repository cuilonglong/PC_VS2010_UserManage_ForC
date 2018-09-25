#pragma once
#include "afxwin.h"
#include "UserInfo.h"

// CAddAmountDlg 对话框

class CAddAmountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAmountDlg)

public:
	CAddAmountDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddAmountDlg();

// 对话框数据
	enum { IDD = IDD_ADD_AMOUNT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int IDNUM;//表示点击的哪个索引的
	int AddAmountTime();//增加续费
	int TranUserInfo(CUserInfo &userinfo,int year,int mon);

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_addamount_year_edit;
	CEdit m_addamount_mon_edit;
};
