#pragma once
#include "afxwin.h"
#include "UserInfo.h"

// CAddAmountDlg �Ի���

class CAddAmountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAmountDlg)

public:
	CAddAmountDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddAmountDlg();

// �Ի�������
	enum { IDD = IDD_ADD_AMOUNT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int IDNUM;//��ʾ������ĸ�������
	int AddAmountTime();//��������
	int TranUserInfo(CUserInfo &userinfo,int year,int mon);

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_addamount_year_edit;
	CEdit m_addamount_mon_edit;
};
