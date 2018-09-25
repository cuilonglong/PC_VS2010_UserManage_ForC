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
	InitComboView();//��ʼ��Combo�ؼ�
	InitDatatimeView();//��ʼ��Datatime�ؼ�
	InitEditContrlView();//��ʼ���༭��ؼ�

	if(Buttonstatus == 1)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("�½��û�");
	}
	//else if(Buttonstatus == 2)
	//{
	//	GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("ɾ���û�");
	//}
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

void CEditUserDlg::InitEditContrlView()//��ʼ���༭��ؼ�
{
	CFile file;
	int num,ret;
	LONGLONG seek;
	CString showString,showString1;
	CUserInfo userinfo;
	BYTE userdata[USERINFOLEN];

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	if(Buttonstatus == 1)//�½��û�
	{
		SetDlgItemText(IDC_EDIT_TIME_STATIC,"�״νɷ�ʱ�䣺");
		int ID = 1;
		do
		{
			ret = FindUserInfo(file,1,ID,"",&seek,&num);//��ѯID�Ƿ����
			if(ret != 0)//��ID����
			{
				break;
			}
			ID++;
		}
		while(ID < USERNUMMAX);
	
		showString.Format("%04d",ID);
		SetDlgItemText(IDC_EDITU_ID_EDIT,showString);
	}
	else if((Buttonstatus == 3)||(Buttonstatus == 4))
	{
		if(Buttonstatus == 3)//�༭�û�
		{
			SetDlgItemText(IDC_EDIT_TIME_STATIC,"���ѽɷ�ʱ�䣺");
			GetDlgItem(IDC_EDITU_ID_EDIT)->EnableWindow(FALSE);//ID���ò��ɱ༭
		}
		else if(Buttonstatus == 4)//�鿴��ϸ��Ϣ
		{
			SetDlgItemText(IDC_EDIT_TIME_STATIC,"�״νɷ�ʱ�䣺");

			GetDlgItem(IDC_EDITU_ID_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_PORT_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_STATUS_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_AMOUNT_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_QQ_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_USER_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_PASSWORD_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
			GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(FALSE);//���ò��ɱ༭
		}
		ret = FindUserInfo(file,1,IDNUM,"",&seek,&num);//��ȡ�����ID��ַ
		if(ret != 0)//��ID������
		{
			return;
		}
		file.Seek(seek,CFile::begin);
		file.Read(userdata,USERINFOLEN);//��ȡ�û�����
		UserDataExplan(userdata,userinfo);//ת��Ϊ�ṹ��

		showString.Format("%04d",userinfo.userid);
		SetDlgItemText(IDC_EDITU_ID_EDIT,showString);//��ʾID
		showString.Format("%04d",userinfo.port);
		SetDlgItemText(IDC_EDITU_PORT_EDIT,showString);//��ʾ�˿�
		showString.Format("%d",userinfo.amount);
		SetDlgItemText(IDC_EDITU_AMOUNT_EDIT,showString);//��ʾ���
		SetDlgItemText(IDC_EDITU_QQ_EDIT,userinfo.qqNum);//��ʾQQ
		SetDlgItemText(IDC_EDITU_PASSWORD_EDIT,userinfo.password);//��ʾ����

		BYTE IP[COMMONDATALEN];
		CAddIPDlg::ReadCommonFile(file,&num,IP);//��ȡ��ȡ�˶���IP
		file.Seek(COMMONDATAADD,CFile::begin);
		file.Read(IP,COMMONDATALEN);
		showString1.Format("%d.%d.%d.%d",IP[userinfo.userip],IP[userinfo.userip+1],IP[userinfo.userip+2],IP[userinfo.userip+3]);
		for(int i = 0;i < num;i++)
		{
			m_edit_user_combo.GetLBText(i,showString);
			if(showString == showString1)
			{
				m_edit_user_combo.SetCurSel(i);
				goto jump;
			}
		}//��ʾIP
		::MessageBox( NULL,_T("��ʾIP���ݳ�������ϵ����Ա��") , TEXT(TiShi) ,MB_OK);
		file.Close();
		return;
jump:		
		if((userinfo.startime == "")||(userinfo.endtime == ""))
		{
			::MessageBox( NULL,_T("��ȡʱ�����ݳ�������ϵ����Ա��") , TEXT(TiShi) ,MB_OK);
			file.Close();
			return;
		}
		SYSTEMTIME time_begin = { 0 }, time_end = { 0 };
		time_end.wYear = _ttoi(userinfo.endtime.Left(4));//��
		time_end.wMonth = _ttoi(userinfo.endtime.Mid(4,2));//��
		time_end.wDay = _ttoi(userinfo.endtime.Mid(6,2));

		if(Buttonstatus == 3)//�༭�û�,��ʾ���һ�νɷ�ʱ��
		{
			time_begin.wYear = _ttoi(userinfo.startime.Left(4));
			time_begin.wMonth = _ttoi(userinfo.startime.Mid(4,2));
			time_begin.wDay = _ttoi(userinfo.startime.Mid(6,2));
		}
		else if(Buttonstatus == 4)//�鿴��ϸ��Ϣ,��ʾ�״�����ʱ��
		{
			time_begin.wYear = _ttoi(userinfo.startime.Left(4));
			time_begin.wMonth = _ttoi(userinfo.startime.Mid(4,2));
			time_begin.wDay = _ttoi(userinfo.startime.Mid(6,2));

			CString showtime;
			showtime.Format("���һ�νɷ�ʱ�䣺\r\n        %04d��%02d��%02d��",_ttoi(userinfo.lasttime.Left(4)),
							_ttoi(userinfo.lasttime.Mid(4,2)),_ttoi(userinfo.lasttime.Mid(6,2)));
			GetDlgItem(IDC_EDIT_INFO_STATIC)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_EDIT_INFO_STATIC,showtime);
		}

		m_datatime_end.SetTime(time_end);
		m_datatime_begain.SetTime(time_begin);//��ʾʱ��
		m_edit_status_combo.SetCurSel(userinfo.userstatus);//��ʾ״̬
	}
	file.Close();
	return;
}

void CEditUserDlg::InitDatatimeView()//��ʼ��Datatime�ؼ�
{
	SYSTEMTIME time_begin = { 0 }, time_end = { 0 };
	
	m_datatime_begain.SetFormat(_T("yyyy-MM-dd"));
	m_datatime_end.SetFormat(_T("yyyy-MM-dd"));
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
	return;
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
	SetDlgItemText(IDC_EDIT_STATUS_COMBO, "ʹ����");
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
	CString ipsting;
	CUserInfo userinfo;
	SYSTEMTIME time_begin = { 0 }, time_end = { 0 };

	if(Buttonstatus == 4)//��ϸ��Ϣ
	{
		goto SUCESS;
	}
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
	if(userinfo.amount < 0)
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
	
	GetDlgItemText(IDC_EDIT_USER_COMBO,ipsting);
	//userinfo.userip = m_edit_user_combo.GetCurSel();
	CAddIPDlg::GetIPSeek(ipsting,&(userinfo.userip));//��ȡIP

	m_datatime_begain.GetTime(&time_begin);
	m_datatime_end.GetTime(&time_end);
	userinfo.endtime.Format("%04d%02d%02d%02d%02d%02d",time_end.wYear,time_end.wMonth,time_end.wDay,
		time_end.wHour,time_end.wMinute,time_end.wSecond);
	userinfo.lasttime.Format("%04d%02d%02d%02d%02d%02d",time_begin.wYear,time_begin.wMonth,time_begin.wDay,
			time_begin.wHour,time_begin.wMinute,time_begin.wSecond);

	userinfo.status = 1;
	if(Buttonstatus == 1)//�½��û�
	{
		userinfo.startime = userinfo.lasttime;
		ret = AddUserInfo(userinfo);
		if(ret != 0)
		{	
			if(ret == 200)//ID�Ƿ�
				::MessageBox( NULL,_T("��������ȷ��ID��1~99999����\r\n\r\n����ʹ���Զ�����ID") , TEXT(TiShi) ,MB_OK);

			goto err;
		}
	}
	else if(Buttonstatus == 3)//�༭�û�
	{
		ret = EditUserInfo(userinfo);
		if(ret != 0)
		{	
			goto err;
		}
	}

SUCESS:
	CDialogEx::OnOK();
err:
	return;
}


void CEditUserDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


/*
���û����ݽ�����ӡ�ɾ�����༭����ȡ�Ȳ���
*/
int CEditUserDlg::AddUserInfo(CUserInfo &userinfo)
{
	CFile file;
	LONGLONG seek;
	CString password;
	CUserInfo userinfo1;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,num;
	
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(ret != 0){
		::MessageBox( NULL,_T("����û���ȡ�û�����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum >= USERNUMMAX)//�û���������
	{
		::MessageBox( NULL,_T("�û������������ޣ�����ϵ����Ա��") , TEXT(TiShi) ,MB_OK);
		ret = 1;
		goto Err;
	}
	
	ret = FindUserInfo(file,1,userinfo.userid,"",&seek,&num);//��ѯID�Ƿ����
	if(ret == 0)//��ID������
	{
		ret = 200;
		goto Err;
	}

	int index = 0;
	do
	{
		CUserInfo userinfo1;
		if(!ReadUserInfo(file,userinfo1,index))//���������Դ洢�û�
		{
			index++;
		}
		else
			break;
	}
	while(index < USERNUMMAX);

	usernum++;
	ret = CUserManageSysDlg::EditFileHead(file,errnum,time,usernum);//�������ӵ��û�����
	if(ret != 0){
		::MessageBox( NULL,_T("����û������û���������ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	ret = UserDataPack(userinfo,userdata);
	if(ret != 0){
		::MessageBox( NULL,_T("����û��������ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	file.Seek(USERDATAADD+ index*USERINFOLEN,CFile::begin);
	file.Write(userdata,USERINFOLEN);
	goto SUCESS;

Err:
	file.Close();
	return ret;

SUCESS:
	::MessageBox( NULL,_T("����û��ɹ���") , TEXT(TiShi) ,MB_OK);
	file.Close();
	return 0;
}

int CEditUserDlg::DelUserInfo(int ID)
{
	CFile file;
	int num,ret;
	LONGLONG seek;
	CString password;
	CUserInfo userinfo;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum;

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	ret = FindUserInfo(file,1,ID,"",&seek,&num);//��ѯID��Ӧ�ĵ�ַ
	if(ret == 0)
	{
		userinfo.status = 0;
		UserDataPack(userinfo,userdata);
		file.Seek(seek,CFile::begin);
		file.Write(userdata,USERINFOLEN);
		ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
		if(ret != 0)
		{
			goto Err;
		}
		usernum--;
		ret = CUserManageSysDlg::EditFileHead(file,errnum,time,usernum);
		if(ret != 0)
		{
			goto Err;
		}
		goto SUCESS;
	}
	else
	{
		goto Err;
	}

Err:
	file.Close();
	return ret;	
SUCESS:
	//::MessageBox( NULL,_T("ɾ���û��ɹ���") , TEXT(TiShi) ,MB_OK);
	file.Close();
	return 0;
}

//���ݴ������ݵ�ID�Զ���λ�޸�
int CEditUserDlg::EditUserInfo(CUserInfo &userinfo)
{
	CFile file;
	LONGLONG seek;
	CString password;
	CUserInfo userinfo1;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,num;
	
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(ret != 0){
		::MessageBox( NULL,_T("�޸��û���ȡ�û�����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum >= USERNUMMAX)//�û���������
	{
		::MessageBox( NULL,_T("�û������������ޣ�����ϵ����Ա��") , TEXT(TiShi) ,MB_OK);
		ret = 1;
		goto Err;
	}
	
	ret = FindUserInfo(file,1,userinfo.userid,"",&seek,&num);//��ѯID�Ƿ�֮ǰ����
	if(ret != 0)//��ID������
	{
		ret = 200;
		goto Err;
	}

	if(userinfo.startime == "")//�༭ʱδ������Ҫ��ȡԭ����
	{
		CUserInfo userinfo1;
		file.Seek(seek,CFile::begin);
		file.Read(userdata,USERINFOLEN);
		UserDataExplan(userdata,userinfo1);
		userinfo.startime = userinfo1.startime;
	}

	ret = UserDataPack(userinfo,userdata);
	if(ret != 0){
		::MessageBox( NULL,_T("�޸��û��������ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	file.Seek(seek,CFile::begin);
	file.Write(userdata,USERINFOLEN);
	goto SUCESS;

Err:
	file.Close();
	return ret;

SUCESS:
	::MessageBox( NULL,_T("�޸��û���Ϣ�ɹ���") , TEXT(TiShi) ,MB_OK);
	file.Close();
	return 0;
}

//���ݴ������ݵ�ID�Զ���λ�޸�(����)
int CEditUserDlg::EditUserInfo(CUserInfo &userinfo,CFile &file)
{
	//CFile file;
	LONGLONG seek;
	CString password;
	CUserInfo userinfo1;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,num;
	
	//file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(ret != 0){
		::MessageBox( NULL,_T("�޸��û���ȡ�û�����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum >= USERNUMMAX)//�û���������
	{
		::MessageBox( NULL,_T("�û������������ޣ�����ϵ����Ա��") , TEXT(TiShi) ,MB_OK);
		ret = 1;
		goto Err;
	}
	
	ret = FindUserInfo(file,1,userinfo.userid,"",&seek,&num);//��ѯID�Ƿ�֮ǰ����
	if(ret != 0)//��ID������
	{
		ret = 200;
		goto Err;
	}

	if(userinfo.startime == "")//�༭ʱδ������Ҫ��ȡԭ����
	{
		CUserInfo userinfo1;
		file.Seek(seek,CFile::begin);
		file.Read(userdata,USERINFOLEN);
		UserDataExplan(userdata,userinfo1);
		userinfo.startime = userinfo1.startime;
	}

	ret = UserDataPack(userinfo,userdata);
	if(ret != 0){
		::MessageBox( NULL,_T("�޸��û��������ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	file.Seek(seek,CFile::begin);
	file.Write(userdata,USERINFOLEN);
	goto SUCESS;

Err:
	//file.Close();
	return ret;

SUCESS:
	//::MessageBox( NULL,_T("�޸��û���Ϣ�ɹ���") , TEXT(TiShi) ,MB_OK);
	//file.Close();
	return 0;
}

/*
���������û�
in
	mode:���Ҹ���
		1��ID
		2���˿�
		3��QQ
		4.�����û�
		5.IP
	ID,Info
		��Ҫ���ҵ���Ϣ
out
	seek:���ص��û��ݶԵ�ַ������ȡID֮�⣬�����Ҫ�������飬��ֹ�������������
	num�����������������û�����

ret
	0:���ҳɹ�
	other��ʧ��
*/
int CEditUserDlg::FindUserInfo(CFile &file,int mode,int IntInfo,CString StrInfo,LONGLONG *seek,int *num)
{
	//CFile file;
	CString password;
	CUserInfo userinfo;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,	i = 0,j=0;
	
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(ret != 0){
		::MessageBox( NULL,_T("�����û���ȡ�û�����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum <= 0)
	{
		ret = 100;
		goto Err;
	}

	*num = 0;
	switch (mode){
	case 1://ID
		*seek = USERDATAADD;
		do
		{
			memset(userdata,0x0,USERINFOLEN);
			file.Seek(*seek + i*USERINFOLEN,CFile::begin);
			file.Read(userdata,USERINFOLEN);
			i++;
			ret = UserDataExplan(userdata,userinfo);
			if((ret == 0)&&(userinfo.status == 1))//�����ɹ�
			{
				if(userinfo.userid == IntInfo)
				{
					(*num) += 1;
					(*seek) += (i-1)*USERINFOLEN;
					goto SUCESS;
				}
				else
				{
					j++;//��ȡ�ɹ�������ƥ��
				}
			}
		}
		while(j != usernum);
		if(j == usernum)//δ���ҵ�
		{
			ret = 100;
			goto Err;
		}
		break;

	case 2://�˿�
		AES::Math_Memset(seek,USERDATAADD,USERNUMMAX);
		do
		{
			memset(userdata,0x0,USERINFOLEN);
			file.Seek(USERDATAADD + i*USERINFOLEN,CFile::begin);
			file.Read(userdata,USERINFOLEN);
			i++;
			ret = UserDataExplan(userdata,userinfo);
			if((ret == 0)&&(userinfo.status == 1))//�����ɹ�
			{
				j++;//��ȡ�ɹ�������һ
				if(userinfo.port == IntInfo)
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//δ���ҵ�
		{
			ret = 100;
			goto Err;
		}
		break;

	case 3://QQ
		AES::Math_Memset(seek,USERDATAADD,USERNUMMAX);
		do
		{
			memset(userdata,0x0,USERINFOLEN);
			file.Seek(USERDATAADD + i*USERINFOLEN,CFile::begin);
			file.Read(userdata,USERINFOLEN);
			i++;
			ret = UserDataExplan(userdata,userinfo);
			if((ret == 0)&&(userinfo.status == 1))//�����ɹ�
			{
				j++;//��ȡ�ɹ�������һ
				if(userinfo.qqNum == StrInfo)
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//δ���ҵ�
		{
			ret = 100;
			goto Err;
		}
		break;

	case 4://�����û�
		AES::Math_Memset(seek,USERDATAADD,USERNUMMAX);
		do
		{
			memset(userdata,0x0,USERINFOLEN);
			file.Seek(USERDATAADD + i*USERINFOLEN,CFile::begin);
			file.Read(userdata,USERINFOLEN);
			i++;
			ret = UserDataExplan(userdata,userinfo);
			if((ret == 0)&&(userinfo.status == 1))//�����ɹ�
			{
				j++;//��ȡ�ɹ�������һ
				if(userinfo.userstatus == IntInfo)//��ѯδ����
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//δ���ҵ�
		{
			ret = 100;
			goto Err;
		}
		break;

	case 5://IP
		AES::Math_Memset(seek,USERDATAADD,USERNUMMAX);
		do
		{
			memset(userdata,0x0,USERINFOLEN);
			file.Seek(USERDATAADD + i*USERINFOLEN,CFile::begin);
			file.Read(userdata,USERINFOLEN);
			i++;
			ret = UserDataExplan(userdata,userinfo);
			if((ret == 0)&&(userinfo.status == 1))//�����ɹ�
			{
				j++;//��ȡ�ɹ�������һ
				if(userinfo.userip == IntInfo)
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//δ���ҵ�
		{
			ret = 100;
			goto Err;
		}
		break;
	default:
		ret = 1;
		goto Err;
	}
Err:
	//file.Close();
	return ret;	

SUCESS:
	//file.Close();
	return 0;
}


/*
��ȡ��Ӧ��ַ���û�����
index:����ƫ���ַ��������USERDATAADD��ʼ��seek*USERINFOLEN�����0��ʼ������
���� 0,����
���� other������
*/
int CEditUserDlg::ReadUserInfo(CFile &file,CUserInfo &userinfo,int index)
{
	int ret;
	//CFile file;
	CString password;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum;
	
	MemsetUserInfo(userinfo);
	//file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(ret != 0){
		//::MessageBox( NULL,_T("��ȡ�û���ȡ�û�����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum == 0)
	{
		ret = 1;
		goto Err;//δ�洢�˻�����
	}
	file.Seek(USERDATAADD+ index*USERINFOLEN,CFile::begin);
	file.Read(userdata,USERINFOLEN);
	ret = UserDataExplan(userdata,userinfo);
	if((ret != 0)||(userinfo.status != 1)){
		//::MessageBox( NULL,_T("��ȡ�û���������ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		ret = 2;
		goto Err;
	}
	else if(userinfo.status == 1)
	{
		goto SUCESS;
	}
Err:
	//file.Close();
	return ret;

SUCESS:
	//::MessageBox( NULL,_T("��ȡ�û��ɹ���") , TEXT(TiShi) ,MB_OK);
	//file.Close();
	return 0;
}

/*
�û����ݽM��BYTE
in:userinfo
out:userdata
*/
int CEditUserDlg::UserDataPack(CUserInfo &userinfo,BYTE *userdata)
{
	int ret;
	uchar key[16];

	memset(userdata,0,USERINFOLEN);
	memcpy(userdata + STATUSADD,(BYTE *)&(userinfo.status),1);
	memcpy(userdata + USERIDADD,(BYTE *)&(userinfo.userid),USERIDLEN);
	memcpy(userdata + PORTADD,(BYTE *)&(userinfo.port),PORTLEN);//�������Ĵ洢

	memcpy(userdata + USERSTATUSADD,(BYTE *)&(userinfo.userstatus),USERSTATUSLEN);
	memcpy(userdata + QQNUMADD,(BYTE *)userinfo.qqNum.GetBuffer(),QQNUMLEN);
	memcpy(userdata + USERIPADD,(BYTE *)&(userinfo.userip),USERIPLEN);
	//memcpy(userdata + USERIPADD,(BYTE *)userinfo.userip.GetBuffer(),USERIPLEN);
	memcpy(userdata + PASWORDADD,(BYTE *)userinfo.password.GetBuffer(),PASWORDLEN);
	memcpy(userdata + AMOUNTADD,(BYTE *)&(userinfo.amount),AMOUNTLEN);
	memcpy(userdata + LASTTIMEADD,(BYTE *)userinfo.lasttime.GetBuffer(),LASTTIMELEN);
	memcpy(userdata + STARTIMEADD,(BYTE *)userinfo.startime.GetBuffer(),STARTIMELEN);
	memcpy(userdata + ENDTIMEADD,(BYTE *)userinfo.endtime.GetBuffer(),ENDTIMELEN);

	ret = AES::AES_GetFeature((uchar *)(LogInUserName+LogInUserName).GetBuffer(),(LogInUserName+LogInUserName).GetLength(),key);
	if(ret != 0){
		return ret;
	}
	ret = AES::AES_Encryption(key,16,userdata + USERSTATUSADD,USERINFOLEN - 9);
	if(ret != 0){
		return ret;
	}
	return 0;
}

/*
�û����ݽ���
in:userdata
out:userinfo
*/
int CEditUserDlg::UserDataExplan(BYTE *userdata,CUserInfo &userinfo)
{
	int ret;
	BYTE data[15];
	uchar key[16];

	ret = AES::AES_GetFeature((uchar *)(LogInUserName+LogInUserName).GetBuffer(),(LogInUserName+LogInUserName).GetLength(),key);
	if(ret != 0){
		return ret;
	}
	memcpy((BYTE *)&(userinfo.status),userdata + STATUSADD,1);
	memcpy((BYTE *)&(userinfo.userid),userdata + USERIDADD,USERIDLEN);
	memcpy((BYTE *)&(userinfo.port),userdata + PORTADD,PORTLEN);//�������Ĵ洢

	ret = AES::AES_Decryption(key,16,userdata + USERSTATUSADD,USERINFOLEN - 9);
	if(ret != 0){
		return ret;
	}
	memcpy((BYTE *)&(userinfo.userstatus),userdata + USERSTATUSADD,USERSTATUSLEN);

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + QQNUMADD,QQNUMLEN);
	userinfo.qqNum.Format("%s",data);//QQ����

	//memset(data,0x00,sizeof(data));
	//memcpy((BYTE *)data,userdata + USERIPADD,USERIPLEN);
	//userinfo.userip.Format("%s",data);
	memcpy((BYTE *)&(userinfo.userip),userdata + USERIPADD,USERIPLEN);//IP

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + PASWORDADD,PASWORDLEN);
	userinfo.password.Format("%s",data);//����

	memcpy((BYTE *)&(userinfo.amount),userdata + AMOUNTADD,AMOUNTLEN);

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + LASTTIMEADD,LASTTIMELEN);
	userinfo.lasttime.Format("%s",data);//���ɷ�ʱ��
	
	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + STARTIMEADD,STARTIMELEN);
	userinfo.startime.Format("%s",data);//����ɷ�ʱ��

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + ENDTIMEADD,ENDTIMELEN);
	userinfo.endtime.Format("%s",data);//����ɷ�ʱ��
	return 0;
}


void CEditUserDlg::MemsetUserInfo(CUserInfo &userinfo)
{
	userinfo.status = -1;
	userinfo.userid = -1;
	userinfo.port = -1;
	userinfo.userstatus = -1;
	userinfo.qqNum = "";
	userinfo.userip = -1;
	userinfo.password = "";
	userinfo.amount = -1;
	userinfo.lasttime = "";
	userinfo.endtime = "";
	userinfo.startime =  "";
	return;
}