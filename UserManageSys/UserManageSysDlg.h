
// UserManageSysDlg.h : ͷ�ļ�
//
#include "FindDlg.h"
#include "EditDlg.h"
#include "UserDefine.h"
#include "UserInfo.h"

#pragma once

#define WindowSizeWide 600
#define WindowSizeHigh 400

// CUserManageSysDlg �Ի���
class CUserManageSysDlg : public CDialogEx
{
// ����
public:
	CUserManageSysDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_USERMANAGESYS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedMainStatic();
	void TabViewInit();
	void ShowTabViewForSel(int sel);
	int CheckPassWorld();
	static CString GetFilePath();
	int LoginUserAccount();
	static int ExplainFileHead(CFile &file,CString account,CString & password,int &errnum,int *time,int &usernum);
	static int EditFileHead(CFile &file,int &errnum,int* time,int usernum = -1);
	int CheckTimeOut24hour(int *time ,SYSTEMTIME st);
	int InitInfoData(CFile &file);

	CFindDlg m_findDlg;
	CEditDlg m_editDlg;
	CTabCtrl m_tab;
	CFont m_status_Font;
	CStatic m_main_static;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
