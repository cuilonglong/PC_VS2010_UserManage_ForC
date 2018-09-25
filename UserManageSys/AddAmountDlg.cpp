// AddAmountDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "AddAmountDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "UserInfo.h"
#include "UserManageSysDlg.h"
#include "EditUserDlg.h"


extern CString LogInUserName;
// CAddAmountDlg �Ի���

IMPLEMENT_DYNAMIC(CAddAmountDlg, CDialogEx)

CAddAmountDlg::CAddAmountDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddAmountDlg::IDD, pParent)
{

}

CAddAmountDlg::~CAddAmountDlg()
{
}

void CAddAmountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDAMOUNT_YEAR_EDIT, m_addamount_year_edit);
	DDX_Control(pDX, IDC_ADDAMOUNT_MON_EDIT, m_addamount_mon_edit);
}


BEGIN_MESSAGE_MAP(CAddAmountDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddAmountDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddAmountDlg ��Ϣ�������


void CAddAmountDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;
	ret = AddAmountTime();
	if((ret == -10)||(ret == -20))//�������ݷǷ�
	{
		::MessageBox( NULL,_T("����ʧ�ܣ�\r\n�������ݷǷ���\r\n���Ӧ���ڵ���0С�ڵ���100\r\n�·�Ӧ����0С��12") , TEXT(TiShi) ,MB_OK);
		return;
	}
	if(ret != 0)
	{
		::MessageBox( NULL,_T("����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		return;
	}
	CDialogEx::OnOK();
}

//���Ѵ���
int CAddAmountDlg::AddAmountTime()
{
	int ret,num;
	CFile file;
	CString str;
	int year,mon;
	LONGLONG seek;
	CUserInfo userinfo;

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	ret = CEditUserDlg::FindUserInfo(file,1,IDNUM,"",&seek,&num);
	if(ret != 0)
	{
		goto err;
	}
	ret = CEditUserDlg::ReadUserInfo(file,userinfo,((int)(seek - USERDATAADD)/USERINFOLEN));
	if(ret != 0)
	{
		goto err;
	}
	
	m_addamount_year_edit.GetWindowText(str);
	year = _ttoi(str);
	m_addamount_mon_edit.GetWindowText(str);
	mon = _ttoi(str);
	ret = TranUserInfo(userinfo,year,mon);
	if(ret != 0)
	{
		goto err;
	}

	ret = CEditUserDlg::EditUserInfo(userinfo,file);//д�����Ѻ���˻�����
	if(ret != 0)
	{
		goto err;
	}

	file.Close();//�ر��ļ���
	return 0;
err:
	file.Close();
	return ret;
}


//ת���û�����
/*
in:
	userinfo:δ����֮ǰ���˻�����
	year��������
	mon��������
out��
	userinfo������֮����˻�����
return
	0:�ɹ�
	other��ʧ��
*/
int CAddAmountDlg::TranUserInfo(CUserInfo &userinfo,int year,int mon)
{
	int ret;
	CTime time;
	int newyear,newmon,newday;//��ȡ���ľ�ʱ��

	if((year > 100)||(year < 0))
	{
		ret = -10;
		goto err;
	}
	if((mon >= 12)||(mon <= 0))
	{
		ret = -20;
		goto err;
	}
	userinfo.userstatus = 0;//ʹ����

	time = CTime::GetCurrentTime();//��ȡϵͳ��ǰʱ��
	userinfo.lasttime.Format("%04d%02d%02d%02d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay(),
		time.GetHour(),time.GetMinute(),time.GetSecond());//�������ɷ�ʱ��
	if((_ttoi(userinfo.endtime.Mid(4,2)) + mon) >12)
	{
		newmon = (_ttoi(userinfo.endtime.Mid(4,2)) + mon) - 12;
		newyear =_ttoi(userinfo.endtime.Left(4)) + 1 + year;
	}
	else
	{
		newmon = (_ttoi(userinfo.endtime.Mid(4,2)) + mon);
		newyear =_ttoi(userinfo.endtime.Left(4)) + year;
	}
	newday =_ttoi(userinfo.endtime.Mid(6,2));
	userinfo.endtime.Format("%04d%02d%02d"+userinfo.endtime.Right(6),newyear,newmon,newday);
	return 0;
err:
	return ret;
}


BOOL CAddAmountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemInt(IDC_ADDAMOUNT_YEAR_EDIT,0);
	SetDlgItemInt(IDC_ADDAMOUNT_MON_EDIT,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
