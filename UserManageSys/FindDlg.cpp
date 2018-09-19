// FindDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "FindDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "AES.h"
#include "Debug.h"

// CFindDlg �Ի���

IMPLEMENT_DYNAMIC(CFindDlg, CDialogEx)

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindDlg::IDD, pParent)
{

}

CFindDlg::~CFindDlg()
{
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIND_COMBO, m_combo);
	DDX_Control(pDX, IDC_FIND_EDIT, m_edit);
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CFindDlg::OnBnClickedFindButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFindDlg ��Ϣ�������


BOOL CFindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	FindViewInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFindDlg::FindViewInit()//��ʼ������
{
	m_combo.AddString(_T("�˿�"));
	m_combo.AddString(_T("QQ"));

	m_combo.SetCurSel(0);
}

void CFindDlg::OnBnClickedFindButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FindString;

	GetDlgItemText(IDC_FIND_EDIT,FindString);
	if(FindString == "")
	{
		MessageBox( TEXT(NInFindInfo) , TEXT(TiShi) ,MB_OK);
		return;
	}

	MessageBox( TEXT(NFUserInfo) , TEXT(TiShi) ,MB_OK);

	/*SetTimer(1, 1000, NULL); */
	return;
}


void CFindDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	uchar data[100],output[16];
	memset(data,0x55,sizeof(data));

	switch (nIDEvent)   
    {   
    case 1:   
		AES::AES_GetFeature(data,sizeof(data),output);
		printf_format((char *)output,sizeof(output));
        break;   

    default:   
        break;   
    }   
	CDialogEx::OnTimer(nIDEvent);
}
