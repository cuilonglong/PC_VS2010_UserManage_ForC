#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "UserInfo.h"


// CEditUserDlg �Ի���

class CEditUserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditUserDlg)

public:
	CEditUserDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditUserDlg();

// �Ի�������
	enum { IDD = IDD_USERINFO_DIALOG };
public:
	int Buttonstatus;//��ʾ�����ĸ���ť

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_edit_user_static;
	CDateTimeCtrl m_datatime_begain;
	CDateTimeCtrl m_datatime_end;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_edit_user_combo;
	afx_msg void OnBnClickedEditSaveButton();
	afx_msg void OnBnClickedCancel();
	void InitComboView();
	static int AddUserInfo(CUserInfo &userinfo);
	static int DelUserInfo(CUserInfo &userinfo);
	static int EditUserInfo(CUserInfo &userinfo);
	static int UserDataPack(CUserInfo &userinfo,BYTE *userdata);
	static int UserDataExplan(BYTE *userdata,CUserInfo &userinfo);

	CComboBox m_edit_status_combo;
};
