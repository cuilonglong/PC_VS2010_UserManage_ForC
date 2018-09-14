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

	static int ReadCommonFile(CFile &file,int *num,BYTE *IP);
	static int AddCommonFile(CFile &file,BYTE *IP);
	static int DelCommonFile(CFile &file,BYTE *IP);

	CIPAddressCtrl m_addip_address;
	int status;//����״̬
	afx_msg void OnBnClickedSaveipButton();
};