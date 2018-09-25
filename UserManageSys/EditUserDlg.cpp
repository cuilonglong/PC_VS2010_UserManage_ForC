// EditUserDlg.cpp : 实现文件
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
// CEditUserDlg 对话框

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


// CEditUserDlg 消息处理程序



BOOL CEditUserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitComboView();//初始化Combo控件
	InitDatatimeView();//初始化Datatime控件
	InitEditContrlView();//初始化编辑框控件

	if(Buttonstatus == 1)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("新建用户");
	}
	//else if(Buttonstatus == 2)
	//{
	//	GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("删除用户");
	//}
	else if(Buttonstatus == 3)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("编辑用户");
	}
	else if(Buttonstatus == 4)
	{
		GetDlgItem(IDC_EDIT_USER_STATIC)->SetWindowText("详细信息");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEditUserDlg::InitEditContrlView()//初始化编辑框控件
{
	CFile file;
	int num,ret;
	LONGLONG seek;
	CString showString,showString1;
	CUserInfo userinfo;
	BYTE userdata[USERINFOLEN];

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	if(Buttonstatus == 1)//新建用户
	{
		SetDlgItemText(IDC_EDIT_TIME_STATIC,"首次缴费时间：");
		int ID = 1;
		do
		{
			ret = FindUserInfo(file,1,ID,"",&seek,&num);//查询ID是否可用
			if(ret != 0)//该ID可用
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
		if(Buttonstatus == 3)//编辑用户
		{
			SetDlgItemText(IDC_EDIT_TIME_STATIC,"续费缴费时间：");
			GetDlgItem(IDC_EDITU_ID_EDIT)->EnableWindow(FALSE);//ID设置不可编辑
		}
		else if(Buttonstatus == 4)//查看详细信息
		{
			SetDlgItemText(IDC_EDIT_TIME_STATIC,"首次缴费时间：");

			GetDlgItem(IDC_EDITU_ID_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_PORT_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_STATUS_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_AMOUNT_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_QQ_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_USER_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITU_PASSWORD_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
			GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(FALSE);//设置不可编辑
		}
		ret = FindUserInfo(file,1,IDNUM,"",&seek,&num);//获取点击的ID地址
		if(ret != 0)//该ID不可用
		{
			return;
		}
		file.Seek(seek,CFile::begin);
		file.Read(userdata,USERINFOLEN);//读取用户数据
		UserDataExplan(userdata,userinfo);//转换为结构体

		showString.Format("%04d",userinfo.userid);
		SetDlgItemText(IDC_EDITU_ID_EDIT,showString);//显示ID
		showString.Format("%04d",userinfo.port);
		SetDlgItemText(IDC_EDITU_PORT_EDIT,showString);//显示端口
		showString.Format("%d",userinfo.amount);
		SetDlgItemText(IDC_EDITU_AMOUNT_EDIT,showString);//显示金额
		SetDlgItemText(IDC_EDITU_QQ_EDIT,userinfo.qqNum);//显示QQ
		SetDlgItemText(IDC_EDITU_PASSWORD_EDIT,userinfo.password);//显示密码

		BYTE IP[COMMONDATALEN];
		CAddIPDlg::ReadCommonFile(file,&num,IP);//获取存取了多少IP
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
		}//显示IP
		::MessageBox( NULL,_T("显示IP数据出错，请联系管理员！") , TEXT(TiShi) ,MB_OK);
		file.Close();
		return;
jump:		
		if((userinfo.startime == "")||(userinfo.endtime == ""))
		{
			::MessageBox( NULL,_T("读取时间数据出错，请联系管理员！") , TEXT(TiShi) ,MB_OK);
			file.Close();
			return;
		}
		SYSTEMTIME time_begin = { 0 }, time_end = { 0 };
		time_end.wYear = _ttoi(userinfo.endtime.Left(4));//年
		time_end.wMonth = _ttoi(userinfo.endtime.Mid(4,2));//月
		time_end.wDay = _ttoi(userinfo.endtime.Mid(6,2));

		if(Buttonstatus == 3)//编辑用户,显示最后一次缴费时间
		{
			time_begin.wYear = _ttoi(userinfo.startime.Left(4));
			time_begin.wMonth = _ttoi(userinfo.startime.Mid(4,2));
			time_begin.wDay = _ttoi(userinfo.startime.Mid(6,2));
		}
		else if(Buttonstatus == 4)//查看详细信息,显示首次续费时间
		{
			time_begin.wYear = _ttoi(userinfo.startime.Left(4));
			time_begin.wMonth = _ttoi(userinfo.startime.Mid(4,2));
			time_begin.wDay = _ttoi(userinfo.startime.Mid(6,2));

			CString showtime;
			showtime.Format("最后一次缴费时间：\r\n        %04d年%02d月%02d日",_ttoi(userinfo.lasttime.Left(4)),
							_ttoi(userinfo.lasttime.Mid(4,2)),_ttoi(userinfo.lasttime.Mid(6,2)));
			GetDlgItem(IDC_EDIT_INFO_STATIC)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_EDIT_INFO_STATIC,showtime);
		}

		m_datatime_end.SetTime(time_end);
		m_datatime_begain.SetTime(time_begin);//显示时间
		m_edit_status_combo.SetCurSel(userinfo.userstatus);//显示状态
	}
	file.Close();
	return;
}

void CEditUserDlg::InitDatatimeView()//初始化Datatime控件
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

void CEditUserDlg::InitComboView()//初始化combo控件
{
	int num;
	BYTE IP[COMMONDATALEN];
	CFile file;
	CString IPString;
	

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	CAddIPDlg::ReadCommonFile(file,&num,IP);
	file.Close();
	for(int i = 0;i < num;i++)
	{
		IPString.Format("%d.%d.%d.%d",IP[4*i],IP[4*i+1],IP[4*i+2],IP[4*i+3]);
		m_edit_user_combo.AddString(IPString);
	}
	m_edit_user_combo.SetCurSel(0);
	SetDlgItemText(IDC_EDIT_USER_COMBO, IPString); //初始化combo控件

	m_edit_status_combo.AddString("使用中");
	m_edit_status_combo.AddString("已关闭");
	m_edit_status_combo.AddString("未续费");
	m_edit_status_combo.AddString("试用中");

	m_edit_status_combo.SetCurSel(0);
	SetDlgItemText(IDC_EDIT_STATUS_COMBO, "使用中");
}

void CEditUserDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	int ret;
	CString ipsting;
	CUserInfo userinfo;
	SYSTEMTIME time_begin = { 0 }, time_end = { 0 };

	if(Buttonstatus == 4)//详细信息
	{
		goto SUCESS;
	}
	userinfo.userid = GetDlgItemInt(IDC_EDITU_ID_EDIT);
	if((userinfo.userid == 0)||(userinfo.userid > 99999))
	{
		::MessageBox( NULL,_T("请输入正确的ID（1~99999）！\r\n\r\n建议使用自动生成ID") , TEXT(TiShi) ,MB_OK);
		return;
	}
	userinfo.port = GetDlgItemInt(IDC_EDITU_PORT_EDIT);
	if((userinfo.port == 0)||(userinfo.port > 9999))
	{
		::MessageBox( NULL,_T("请输入正确的端口（1~9999）！") , TEXT(TiShi) ,MB_OK);
		return;
	}
	userinfo.amount = GetDlgItemInt(IDC_EDITU_AMOUNT_EDIT);
	if(userinfo.amount < 0)
	{
		::MessageBox( NULL,_T("请输入正确的金额数值！") , TEXT(TiShi) ,MB_OK);
		return;
	}
	GetDlgItemText(IDC_EDITU_QQ_EDIT,userinfo.qqNum);
	if((userinfo.qqNum == "")||(userinfo.qqNum.GetLength() > 12))
	{
		::MessageBox( NULL,_T("请输入正确的QQ号码！") , TEXT(TiShi) ,MB_OK);
		return;
	}
	GetDlgItemText(IDC_EDITU_PASSWORD_EDIT,userinfo.password);
	if((userinfo.password == "")||(userinfo.password.GetLength() > 15))
	{
		::MessageBox( NULL,_T("请输入正确的密码长度(小于15位)！") , TEXT(TiShi) ,MB_OK);
		return;
	}
	userinfo.userstatus = m_edit_status_combo.GetCurSel();
	
	GetDlgItemText(IDC_EDIT_USER_COMBO,ipsting);
	//userinfo.userip = m_edit_user_combo.GetCurSel();
	CAddIPDlg::GetIPSeek(ipsting,&(userinfo.userip));//获取IP

	m_datatime_begain.GetTime(&time_begin);
	m_datatime_end.GetTime(&time_end);
	userinfo.endtime.Format("%04d%02d%02d%02d%02d%02d",time_end.wYear,time_end.wMonth,time_end.wDay,
		time_end.wHour,time_end.wMinute,time_end.wSecond);
	userinfo.lasttime.Format("%04d%02d%02d%02d%02d%02d",time_begin.wYear,time_begin.wMonth,time_begin.wDay,
			time_begin.wHour,time_begin.wMinute,time_begin.wSecond);

	userinfo.status = 1;
	if(Buttonstatus == 1)//新建用户
	{
		userinfo.startime = userinfo.lasttime;
		ret = AddUserInfo(userinfo);
		if(ret != 0)
		{	
			if(ret == 200)//ID非法
				::MessageBox( NULL,_T("请输入正确的ID（1~99999）！\r\n\r\n建议使用自动生成ID") , TEXT(TiShi) ,MB_OK);

			goto err;
		}
	}
	else if(Buttonstatus == 3)//编辑用户
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
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


/*
对用户数据进行添加、删除、编辑、读取等操作
*/
int CEditUserDlg::AddUserInfo(CUserInfo &userinfo)
{
	CFile file;
	LONGLONG seek;
	CString password;
	CUserInfo userinfo1;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,num;
	
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//获取已经存储的用户个数
	if(ret != 0){
		::MessageBox( NULL,_T("添加用户获取用户数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum >= USERNUMMAX)//用户超过上限
	{
		::MessageBox( NULL,_T("用户数量超过上限，请联系管理员！") , TEXT(TiShi) ,MB_OK);
		ret = 1;
		goto Err;
	}
	
	ret = FindUserInfo(file,1,userinfo.userid,"",&seek,&num);//查询ID是否可用
	if(ret == 0)//该ID不可用
	{
		ret = 200;
		goto Err;
	}

	int index = 0;
	do
	{
		CUserInfo userinfo1;
		if(!ReadUserInfo(file,userinfo1,index))//该索引可以存储用户
		{
			index++;
		}
		else
			break;
	}
	while(index < USERNUMMAX);

	usernum++;
	ret = CUserManageSysDlg::EditFileHead(file,errnum,time,usernum);//更新增加的用户个数
	if(ret != 0){
		::MessageBox( NULL,_T("添加用户设置用户个数数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	ret = UserDataPack(userinfo,userdata);
	if(ret != 0){
		::MessageBox( NULL,_T("添加用户组包数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	file.Seek(USERDATAADD+ index*USERINFOLEN,CFile::begin);
	file.Write(userdata,USERINFOLEN);
	goto SUCESS;

Err:
	file.Close();
	return ret;

SUCESS:
	::MessageBox( NULL,_T("添加用户成功！") , TEXT(TiShi) ,MB_OK);
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

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	ret = FindUserInfo(file,1,ID,"",&seek,&num);//查询ID对应的地址
	if(ret == 0)
	{
		userinfo.status = 0;
		UserDataPack(userinfo,userdata);
		file.Seek(seek,CFile::begin);
		file.Write(userdata,USERINFOLEN);
		ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//获取已经存储的用户个数
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
	//::MessageBox( NULL,_T("删除用户成功！") , TEXT(TiShi) ,MB_OK);
	file.Close();
	return 0;
}

//根据传入数据的ID自动定位修改
int CEditUserDlg::EditUserInfo(CUserInfo &userinfo)
{
	CFile file;
	LONGLONG seek;
	CString password;
	CUserInfo userinfo1;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,num;
	
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//获取已经存储的用户个数
	if(ret != 0){
		::MessageBox( NULL,_T("修改用户获取用户数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum >= USERNUMMAX)//用户超过上限
	{
		::MessageBox( NULL,_T("用户数量超过上限，请联系管理员！") , TEXT(TiShi) ,MB_OK);
		ret = 1;
		goto Err;
	}
	
	ret = FindUserInfo(file,1,userinfo.userid,"",&seek,&num);//查询ID是否之前存在
	if(ret != 0)//该ID不可用
	{
		ret = 200;
		goto Err;
	}

	if(userinfo.startime == "")//编辑时未传入需要读取原来的
	{
		CUserInfo userinfo1;
		file.Seek(seek,CFile::begin);
		file.Read(userdata,USERINFOLEN);
		UserDataExplan(userdata,userinfo1);
		userinfo.startime = userinfo1.startime;
	}

	ret = UserDataPack(userinfo,userdata);
	if(ret != 0){
		::MessageBox( NULL,_T("修改用户组包数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	file.Seek(seek,CFile::begin);
	file.Write(userdata,USERINFOLEN);
	goto SUCESS;

Err:
	file.Close();
	return ret;

SUCESS:
	::MessageBox( NULL,_T("修改用户信息成功！") , TEXT(TiShi) ,MB_OK);
	file.Close();
	return 0;
}

//根据传入数据的ID自动定位修改(重载)
int CEditUserDlg::EditUserInfo(CUserInfo &userinfo,CFile &file)
{
	//CFile file;
	LONGLONG seek;
	CString password;
	CUserInfo userinfo1;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,num;
	
	//file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//获取已经存储的用户个数
	if(ret != 0){
		::MessageBox( NULL,_T("修改用户获取用户数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum >= USERNUMMAX)//用户超过上限
	{
		::MessageBox( NULL,_T("用户数量超过上限，请联系管理员！") , TEXT(TiShi) ,MB_OK);
		ret = 1;
		goto Err;
	}
	
	ret = FindUserInfo(file,1,userinfo.userid,"",&seek,&num);//查询ID是否之前存在
	if(ret != 0)//该ID不可用
	{
		ret = 200;
		goto Err;
	}

	if(userinfo.startime == "")//编辑时未传入需要读取原来的
	{
		CUserInfo userinfo1;
		file.Seek(seek,CFile::begin);
		file.Read(userdata,USERINFOLEN);
		UserDataExplan(userdata,userinfo1);
		userinfo.startime = userinfo1.startime;
	}

	ret = UserDataPack(userinfo,userdata);
	if(ret != 0){
		::MessageBox( NULL,_T("修改用户组包数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	file.Seek(seek,CFile::begin);
	file.Write(userdata,USERINFOLEN);
	goto SUCESS;

Err:
	//file.Close();
	return ret;

SUCESS:
	//::MessageBox( NULL,_T("修改用户信息成功！") , TEXT(TiShi) ,MB_OK);
	//file.Close();
	return 0;
}

/*
遍历查找用户
in
	mode:查找根据
		1：ID
		2：端口
		3：QQ
		4.过期用户
		5.IP
	ID,Info
		需要查找的信息
out
	seek:返回的用户据对地址（除获取ID之外，别的需要传入数组，防止多个符合条件）
	num：符合搜索条件的用户个数

ret
	0:查找成功
	other：失败
*/
int CEditUserDlg::FindUserInfo(CFile &file,int mode,int IntInfo,CString StrInfo,LONGLONG *seek,int *num)
{
	//CFile file;
	CString password;
	CUserInfo userinfo;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum,ret,	i = 0,j=0;
	
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//获取已经存储的用户个数
	if(ret != 0){
		::MessageBox( NULL,_T("查找用户获取用户数据失败！") , TEXT(TiShi) ,MB_OK);
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
			if((ret == 0)&&(userinfo.status == 1))//解析成功
			{
				if(userinfo.userid == IntInfo)
				{
					(*num) += 1;
					(*seek) += (i-1)*USERINFOLEN;
					goto SUCESS;
				}
				else
				{
					j++;//读取成功，但不匹配
				}
			}
		}
		while(j != usernum);
		if(j == usernum)//未查找到
		{
			ret = 100;
			goto Err;
		}
		break;

	case 2://端口
		AES::Math_Memset(seek,USERDATAADD,USERNUMMAX);
		do
		{
			memset(userdata,0x0,USERINFOLEN);
			file.Seek(USERDATAADD + i*USERINFOLEN,CFile::begin);
			file.Read(userdata,USERINFOLEN);
			i++;
			ret = UserDataExplan(userdata,userinfo);
			if((ret == 0)&&(userinfo.status == 1))//解析成功
			{
				j++;//读取成功计数加一
				if(userinfo.port == IntInfo)
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//未查找到
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
			if((ret == 0)&&(userinfo.status == 1))//解析成功
			{
				j++;//读取成功计数加一
				if(userinfo.qqNum == StrInfo)
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//未查找到
		{
			ret = 100;
			goto Err;
		}
		break;

	case 4://过期用户
		AES::Math_Memset(seek,USERDATAADD,USERNUMMAX);
		do
		{
			memset(userdata,0x0,USERINFOLEN);
			file.Seek(USERDATAADD + i*USERINFOLEN,CFile::begin);
			file.Read(userdata,USERINFOLEN);
			i++;
			ret = UserDataExplan(userdata,userinfo);
			if((ret == 0)&&(userinfo.status == 1))//解析成功
			{
				j++;//读取成功计数加一
				if(userinfo.userstatus == IntInfo)//查询未续费
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//未查找到
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
			if((ret == 0)&&(userinfo.status == 1))//解析成功
			{
				j++;//读取成功计数加一
				if(userinfo.userip == IntInfo)
				{
					seek[*num] += ((i-1)*USERINFOLEN);
					(*num) += 1;
				}
			}
		}
		while(j != usernum);
		if((j == usernum)&&(*num == 0))//未查找到
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
读取对应地址的用户数据
index:绝对偏差地址倍数，从USERDATAADD开始，seek*USERINFOLEN（须从0开始遍历）
返回 0,存在
返回 other不存在
*/
int CEditUserDlg::ReadUserInfo(CFile &file,CUserInfo &userinfo,int index)
{
	int ret;
	//CFile file;
	CString password;
	BYTE userdata[USERINFOLEN];
	int errnum,time[4],usernum;
	
	MemsetUserInfo(userinfo);
	//file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//获取已经存储的用户个数
	if(ret != 0){
		//::MessageBox( NULL,_T("读取用户获取用户数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum == 0)
	{
		ret = 1;
		goto Err;//未存储账户数据
	}
	file.Seek(USERDATAADD+ index*USERINFOLEN,CFile::begin);
	file.Read(userdata,USERINFOLEN);
	ret = UserDataExplan(userdata,userinfo);
	if((ret != 0)||(userinfo.status != 1)){
		//::MessageBox( NULL,_T("读取用户解析数据失败！") , TEXT(TiShi) ,MB_OK);
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
	//::MessageBox( NULL,_T("读取用户成功！") , TEXT(TiShi) ,MB_OK);
	//file.Close();
	return 0;
}

/*
用户数据組包BYTE
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
	memcpy(userdata + PORTADD,(BYTE *)&(userinfo.port),PORTLEN);//以上明文存储

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
用户数据解析
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
	memcpy((BYTE *)&(userinfo.port),userdata + PORTADD,PORTLEN);//以上明文存储

	ret = AES::AES_Decryption(key,16,userdata + USERSTATUSADD,USERINFOLEN - 9);
	if(ret != 0){
		return ret;
	}
	memcpy((BYTE *)&(userinfo.userstatus),userdata + USERSTATUSADD,USERSTATUSLEN);

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + QQNUMADD,QQNUMLEN);
	userinfo.qqNum.Format("%s",data);//QQ号码

	//memset(data,0x00,sizeof(data));
	//memcpy((BYTE *)data,userdata + USERIPADD,USERIPLEN);
	//userinfo.userip.Format("%s",data);
	memcpy((BYTE *)&(userinfo.userip),userdata + USERIPADD,USERIPLEN);//IP

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + PASWORDADD,PASWORDLEN);
	userinfo.password.Format("%s",data);//密码

	memcpy((BYTE *)&(userinfo.amount),userdata + AMOUNTADD,AMOUNTLEN);

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + LASTTIMEADD,LASTTIMELEN);
	userinfo.lasttime.Format("%s",data);//最后缴费时间
	
	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + STARTIMEADD,STARTIMELEN);
	userinfo.startime.Format("%s",data);//最初缴费时间

	memset(data,0x00,sizeof(data));
	memcpy((BYTE *)data,userdata + ENDTIMEADD,ENDTIMELEN);
	userinfo.endtime.Format("%s",data);//最初缴费时间
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