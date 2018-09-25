// AddAmountDlg.cpp : 实现文件
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
// CAddAmountDlg 对话框

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


// CAddAmountDlg 消息处理程序


void CAddAmountDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret;
	ret = AddAmountTime();
	if((ret == -10)||(ret == -20))//输入数据非法
	{
		::MessageBox( NULL,_T("续费失败！\r\n输入数据非法！\r\n年份应大于等于0小于等于100\r\n月份应大于0小于12") , TEXT(TiShi) ,MB_OK);
		return;
	}
	if(ret != 0)
	{
		::MessageBox( NULL,_T("续费失败！") , TEXT(TiShi) ,MB_OK);
		return;
	}
	CDialogEx::OnOK();
}

//续费处理
int CAddAmountDlg::AddAmountTime()
{
	int ret,num;
	CFile file;
	CString str;
	int year,mon;
	LONGLONG seek;
	CUserInfo userinfo;

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
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

	ret = CEditUserDlg::EditUserInfo(userinfo,file);//写入续费后的账户数据
	if(ret != 0)
	{
		goto err;
	}

	file.Close();//关闭文件流
	return 0;
err:
	file.Close();
	return ret;
}


//转换用户数据
/*
in:
	userinfo:未续费之前的账户数据
	year：续费年
	mon：续费月
out：
	userinfo：续费之后的账户数据
return
	0:成功
	other：失败
*/
int CAddAmountDlg::TranUserInfo(CUserInfo &userinfo,int year,int mon)
{
	int ret;
	CTime time;
	int newyear,newmon,newday;//读取出的旧时间

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
	userinfo.userstatus = 0;//使用中

	time = CTime::GetCurrentTime();//获取系统当前时间
	userinfo.lasttime.Format("%04d%02d%02d%02d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay(),
		time.GetHour(),time.GetMinute(),time.GetSecond());//更新最后缴费时间
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

	// TODO:  在此添加额外的初始化
	SetDlgItemInt(IDC_ADDAMOUNT_YEAR_EDIT,0);
	SetDlgItemInt(IDC_ADDAMOUNT_MON_EDIT,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
