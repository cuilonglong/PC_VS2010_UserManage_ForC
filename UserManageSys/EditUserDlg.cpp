// EditUserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "EditUserDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "UserInfo.h"
#include "Debug.h"
#include "AddIPDlg.h"
#include "UserManageSysDlg.h"
#include "AES.h"

extern CString LogInUserName;
// CEditUserDlg �Ի���

IMPLEMENT_DYNAMIC(CEditUserDlg, CDialogEx)

CEditUserDlg::CEditUserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditUserDlg::IDD, pParent)
{

}

CEditUserDlg::~CEditUserDlg()
{
}

void CEditUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USER_STATIC, m_edit_user_static);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datatime_begain);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_datatime_end);
	DDX_Control(pDX, IDC_EDIT_USER_COMBO, m_edit_user_combo);
	DDX_Control(pDX, IDC_EDIT_STATUS_COMBO, m_edit_status_combo);
}


BEGIN_MESSAGE_MAP(CEditUserDlg, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CEditUserDlg::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_EDIT_SAVE_BUTTON, &CEditUserDlg::OnBnClickedEditSaveButton)
	ON_BN_CLICKED(IDCANCEL, &CEditUserDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEditUserDlg ��Ϣ�������



BOOL CEditUserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitComboView();
	m_datatime_begain.SetFormat(_T("yyyy-MM-dd"));
	m_datatime_end.SetFormat(_T("yyyy-MM-dd"));

	if(Buttonstatus == 1)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("�½��û�");
	}
	else if(Buttonstatus == 2)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("ɾ���û�");
	}
	else if(Buttonstatus == 3)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("�༭�û�");
	}
	else if(Buttonstatus == 4)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("��ϸ��Ϣ");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEditUserDlg::InitComboView()//��ʼ��combo�ؼ�
{
	int num;
	BYTE IP[COMMONDATALEN];
	CFile file;
	CString IPString;
	

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	CAddIPDlg::ReadCommonFile(file,&num,IP);
	file.Close();
	for(int i = 0;i < num;i++)
	{
		IPString.Format("%d.%d.%d.%d",IP[4*i],IP[4*i+1],IP[4*i+2],IP[4*i+3]);
		m_edit_user_combo.AddString(IPString);
	}
	m_edit_user_combo.SetCurSel(0);
	SetDlgItemText(IDC_EDIT_USER_COMBO, IPString); //��ʼ��combo�ؼ�

	m_edit_status_combo.AddString("ʹ����");
	m_edit_status_combo.AddString("�ѹر�");
	m_edit_status_combo.AddString("δ����");
	m_edit_status_combo.AddString("������");

	m_edit_status_combo.SetCurSel(0);
	SetDlgItemText(IDC_EDIT_STATUS_COMBO, "��ͨ");
}

void CEditUserDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SYSTEMTIME time_begin = { 0 }, time_end = { 0 };

	m_datatime_begain.GetTime(&time_begin);
	time_end = time_begin;
	if(time_end.wMonth != 12)
	{
		time_end.wMonth += 1;
	}
	else
	{
		time_end.wMonth = 1;
		time_end.wYear += 1;  
	}

	m_datatime_end.SetTime(time_end);

	*pResult = 0;
}


void CEditUserDlg::OnBnClickedEditSaveButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;
	CUserInfo userinfo;

	userinfo.userid = GetDlgItemInt(IDC_EDITU_ID_EDIT);
	if((userinfo.userid == 0)||(userinfo.userid > 99999))
	{
		::MessageBox( NULL,_T("��������ȷ��ID��1~99999����\r\n\r\n����ʹ���Զ�����ID") , TEXT(TiShi) ,MB_OK);
		return;
	}
	userinfo.port = GetDlgItemInt(IDC_EDITU_PORT_EDIT);
	if((userinfo.port == 0)||(userinfo.port > 9999))
	{
		::MessageBox( NULL,_T("��������ȷ�Ķ˿ڣ�1~9999����") , TEXT(TiShi) ,MB_OK);
		return;
	}
	userinfo.amount = GetDlgItemInt(IDC_EDITU_AMOUNT_EDIT);
	if(userinfo.amount <= 0)
	{
		::MessageBox( NULL,_T("��������ȷ�Ľ����ֵ��") , TEXT(TiShi) ,MB_OK);
		return;
	}
	GetDlgItemText(IDC_EDITU_QQ_EDIT,userinfo.qqNum);
	if((userinfo.qqNum == "")||(userinfo.qqNum.GetLength() > 12))
	{
		::MessageBox( NULL,_T("��������ȷ��QQ���룡") , TEXT(TiShi) ,MB_OK);
		return;
	}
	GetDlgItemText(IDC_EDITU_PASSWORD_EDIT,userinfo.password);
	if((userinfo.password == "")||(userinfo.password.GetLength() > 15))
	{
		::MessageBox( NULL,_T("��������ȷ�����볤��(С��15λ)��") , TEXT(TiShi) ,MB_OK);
		return;
	}
	userinfo.userstatus = m_edit_status_combo.GetCurSel();
	GetDlgItemText(IDC_EDIT_USER_COMBO,userinfo.userip);
	
	SYSTEMTIME time_begin = { 0 }, time_end = { 0 };
	m_datatime_begain.GetTime(&time_begin);
	m_datatime_end.GetTime(&time_end);
	userinfo.endtime.Format("%04d%02d%02d%02d%02d%02d",time_end.wYear,time_end.wMonth,time_end.wDay,
		time_end.wHour,time_end.wMinute,time_end.wSecond);
	userinfo.lasttime.Format("%04d%02d%02d%02d%02d%02d",time_begin.wYear,time_begin.wMonth,time_begin.wDay,
			time_begin.wHour,time_begin.wMinute,time_begin.wSecond);
	if(Buttonstatus == 1)//�½��û�
	{
		userinfo.startime = userinfo.lasttime;
		userinfo.status = 1;
		ret = AddUserInfo(userinfo);
	}
	CDialogEx::OnOK();
}


void CEditUserDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


/*
���û����ݽ�����ӡ�ɾ�����༭�Ȳ���
*/
int CEditUserDlg::AddUserInfo(CUserInfo &userinfo)
{
	CFile file;
	CString password;
	int errnum,time[4],usernum;
	BYTE userdata[USERINFOLEN];
	
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(usernum >= USERNUMMAX)//�û���������
	{
		::MessageBox( NULL,_T("�û������������ޣ�����ϵ����Ա��") , TEXT(TiShi) ,MB_OK);
		file.Close();
		return 1;
	}
	usernum++;
	CUserManageSysDlg::EditFileHead(file,errnum,time,usernum);//�������ӵ��û�����
	UserDataPack(userinfo,userdata);
	file.Seek(USERDATAADD+(usernum -1)*USERINFOLEN,CFile::begin);
	file.Write(userdata,USERINFOLEN);
	file.Close();
	return 0;
}

int CEditUserDlg::DelUserInfo(CUserInfo &userinfo)
{
	return 0;
}

int CEditUserDlg::EditUserInfo(CUserInfo &userinfo)
{
	return 0;
}

/*
�û����ݽM��
*/
int CEditUserDlg::UserDataPack(CUserInfo &userinfo,BYTE *userdata)
{
	uchar key[16];

	memset(userdata,0,USERINFOLEN);
	memcpy(userdata + STATUSADD,(BYTE *)&(userinfo.status),1);
	memcpy(userdata + USERIDADD,(BYTE *)&(userinfo.userid),USERIDLEN);//�������Ĵ洢
	memcpy(userdata + PORTADD,(BYTE *)&(userinfo.port),PORTLEN);
	memcpy(userdata + USERSTATUSADD,(BYTE *)&(userinfo.userstatus),USERSTATUSLEN);
	memcpy(userdata + QQNUMADD,(BYTE *)userinfo.qqNum.GetBuffer(),QQNUMLEN);
	memcpy(userdata + USERIPADD,(BYTE *)userinfo.userip.GetBuffer(),USERIPLEN);
	memcpy(userdata + PASWORDADD,(BYTE *)userinfo.password.GetBuffer(),PASWORDLEN);
	memcpy(userdata + AMOUNTADD,(BYTE *)&(userinfo.amount),AMOUNTLEN);
	memcpy(userdata + LASTTIMEADD,(BYTE *)userinfo.lasttime.GetBuffer(),LASTTIMELEN);
	memcpy(userdata + STARTIMEADD,(BYTE *)userinfo.startime.GetBuffer(),STARTIMELEN);
	memcpy(userdata + ENDTIMEADD,(BYTE *)userinfo.endtime.GetBuffer(),ENDTIMELEN);

	AES::AES_GetFeature((uchar *)(LogInUserName+LogInUserName).GetBuffer(),(LogInUserName+LogInUserName).GetLength(),key);
	AES::AES_Encryption(key,16,userdata + PORTADD,USERINFOLEN - 9);
	return 0;
}

/*
�û����ݽ���
*/
int CEditUserDlg::UserDataExplan(BYTE *userdata,CUserInfo &userinfo)
{
	return 0;
}
