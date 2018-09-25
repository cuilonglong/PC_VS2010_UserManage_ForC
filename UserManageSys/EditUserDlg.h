#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "UserInfo.h"


// CEditUserDlg 对话框

class CEditUserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditUserDlg)

public:
	CEditUserDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditUserDlg();

// 对话框数据
	enum { IDD = IDD_USERINFO_DIALOG };
public:
	int Buttonstatus;//表示按下哪个按钮
	int IDNUM;//表示点击的哪个索引的
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	void InitDatatimeView();
	void InitEditContrlView();//初始化编辑框控件

	static int AddUserInfo(CUserInfo &userinfo);
	static int DelUserInfo(int ID);
	static int EditUserInfo(CUserInfo &userinfo);
	static int EditUserInfo(CUserInfo &userinfo,CFile &file);
	static int ReadUserInfo(CFile &file,CUserInfo &userinfo,int index);
	static int FindUserInfo(CFile &file,int mode,int ID,CString Info,LONGLONG *seek,int *num);
	static int UserDataPack(CUserInfo &userinfo,BYTE *userdata);
	static int UserDataExplan(BYTE *userdata,CUserInfo &userinfo);
	static void MemsetUserInfo(CUserInfo &userinfo);

	CComboBox m_edit_status_combo;
};
