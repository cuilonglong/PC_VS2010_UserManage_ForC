#pragma once
#include "afxcmn.h"


// CAddIPDlg 对话框

class CAddIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddIPDlg)

public:
	CAddIPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddIPDlg();

// 对话框数据
	enum { IDD = IDD_ADDIP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	static CString GetCommonIPForSel(CFile &file,int sel);
	static int ReadCommonFile(CFile &file,int *num,BYTE *IP);
	static int AddCommonFile(CFile &file,BYTE *IP);
	static int DelCommonFile(CFile &file,BYTE *IP);
	static int EditCommonFile(CFile &file,BYTE *IP,BYTE *IP1);
	static int GetIPSeek(CString IP,int *seek);
	static int GetIPSeek(CString IP,int *seek,CFile &file);

	CIPAddressCtrl m_addip_address;
	int status;//添加状态
	afx_msg void OnBnClickedSaveipButton();
	CIPAddressCtrl m_addip_address1;
};