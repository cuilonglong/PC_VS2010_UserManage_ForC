// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "UserManageSysDlg.h"
#include "UserInfo.h"
#include "AES.h"
#include "Debug.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	GetDlgItemText(IDC_LOGINACCOUNT_EDIT,account);
	GetDlgItemText(IDC_LOGINPASSWORLD1_EDIT,password1);
	GetDlgItemText(IDC_LOGINPASSWORLD2_EDIT,password2);

	if((account == "")||(password1 == "")||(password2 == ""))//限制不为空
	{
		status = 1;
		MessageBox( _T(NIUserInfo) , TEXT(TiShi) ,MB_OK);
		return;
	}

	if(((account.GetLength() < 4)&&(account.GetLength() > 12))
		||((password1.GetLength() < 6)&&(password1.GetLength() > 14)))//限制账号密码长度
	{
		status = 2;
		MessageBox( _T(UserLenErr) , TEXT(TiShi) ,MB_OK);
		return;
	}

	DWORD dwAttrib = GetFileAttributes(CUserManageSysDlg::GetFilePath()+account+FileSuffix);//获取文件属性
	if((INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))  //不允许重复创建
	{
		status = 3;
		MessageBox( _T(UserExtern) , TEXT(TiShi) ,MB_OK);
		return;	
	}

	if(password1 != password2)
	{
		status = 4;
		MessageBox( _T(LoginNSame) , TEXT(TiShi) ,MB_OK);
		return;
	}
	else
	{
		//status = 0;
		status = CreatUserFile(account,password1);//创建

	}
	
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if((account != "")||(password1 != "")||(password2 != ""))//保存上次输入数据
	{
		SetDlgItemText(IDC_LOGINACCOUNT_EDIT,account);
		SetDlgItemText(IDC_LOGINPASSWORLD1_EDIT,password1);
		SetDlgItemText(IDC_LOGINPASSWORLD2_EDIT,password2);	
	}

	if(status == 2)//账户长度输入有问题
	{
		GetDlgItem(IDC_LOGIN_STATIC)->ShowWindow(TRUE);//显示该控件文字提示长度不对
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



/*
	存储head文件，初始化
*/
int CLoginDlg::CreatUserFile(CString accountm,CString password)
{
	CFile file;
	DataHeard datahead;
	uchar data[32],key[16],headkcv[USERINFOHEADLEN-16];
	int seeklen = 0;

	if(file.Open(CUserManageSysDlg::GetFilePath()+account+FileSuffix, CFile::modeCreate | CFile::modeReadWrite))
	{
		datahead.Account = accountm;
		datahead.Passworld = password;
		datahead.AccountLen = accountm.GetLength();
		datahead.PassworldLen = password.GetLength();
		datahead.InputErrNum = 0;
		datahead.UserNum = 0;
		memset((int *)&(datahead.Time),0x00,4*sizeof(int));
		datahead.headlen = 5*sizeof(int)+32*2+16+16;//16字节的时间，年月日;最后16字节HCV校验值
		
		AES::AES_GetFeature((uchar *)(accountm+accountm).GetBuffer(),(accountm+accountm).GetLength(),key);
		file.SeekToBegin(); 
		file.Write((int *)&(datahead.headlen),sizeof(int));
		memcpy(headkcv+seeklen,(uchar *)&(datahead.headlen),sizeof(int));
		seeklen += sizeof(int);//存储head


		//file.Seek(seeklen,CFile::begin);
		file.Write((int *)&(datahead.AccountLen),sizeof(int));
		memcpy(headkcv+seeklen,(uchar *)&(datahead.AccountLen),sizeof(int));
		seeklen += sizeof(int);//存储账号长度

		//file.Seek(seeklen,CFile::begin);
		memset(data,0x00,sizeof(data));
		memcpy(data,accountm.GetBuffer(),accountm.GetLength());
		AES::AES_Encryption(key,16,data,32);
		file.Write(data,32);
		memcpy(headkcv+seeklen,data,32);
		seeklen += 32;//存储账号

		//file.Seek(seeklen,CFile::begin);
		file.Write((int *)&(datahead.PassworldLen),sizeof(int));
		memcpy(headkcv+seeklen,(uchar *)&(datahead.PassworldLen),sizeof(int));
		seeklen += sizeof(int);//存储密码长度


		//file.Seek(seeklen,CFile::begin);
		memset(data,0x00,sizeof(data));
		memcpy(data,password.GetBuffer(),password.GetLength());
		AES::AES_Encryption(key,16,data,32);
		file.Write(data,32);
		memcpy(headkcv+seeklen,data,32);
		seeklen += 32;//存储密码

		//file.Seek(seeklen,CFile::begin); 
		file.Write((int *)&(datahead.InputErrNum),sizeof(int));
		memcpy(headkcv+seeklen,(uchar *)&(datahead.InputErrNum),sizeof(int));
		seeklen += sizeof(int);//存储InputErrNum

		//file.Seek(seeklen,CFile::begin); 
		file.Write((int *)&(datahead.UserNum),sizeof(int));
		memcpy(headkcv+seeklen,(uchar *)&(datahead.UserNum),sizeof(int));
		seeklen += sizeof(int);//存储UserNum

		file.Write((int *)&(datahead.Time),4*sizeof(int));
		memcpy(headkcv+seeklen,(uchar *)&(datahead.Time),4*sizeof(int));
		seeklen += (4*sizeof(int));//预留时间空间 年月日时

		AES::AES_GetFeature((uchar *)headkcv,seeklen,(uchar *)datahead.HCV);
		//printf_format((char *)headkcv,seeklen);

		file.Write((uchar *)datahead.HCV,16);
		file.Close();//关闭文件
	}
	else
	{
		CString string;
		string.Format("%s [%s]",LoginErr, ErrCord2001); //将变量组装到字符串中
		MessageBox( _T(string) , TEXT(TiShi) ,MB_OK | MB_ICONHAND );
		return -1;
	}

	return 0;
}