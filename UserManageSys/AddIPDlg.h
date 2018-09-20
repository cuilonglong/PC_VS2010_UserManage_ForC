#pragma once
#include "afxcmn.h"


// CAddIPDlg �Ի���

class CAddIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddIPDlg)

public:
	CAddIPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddIPDlg();

// �Ի�������
	enum { IDD = IDD_ADDIP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	int status;//���״̬
	afx_msg void OnBnClickedSaveipButton();
	CIPAddressCtrl m_addip_address1;
};