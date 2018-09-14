// AddIPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "AddIPDlg.h"
#include "afxdialogex.h"
#include "Debug.h"
#include "UserDefine.h"
#include "UserManageSysDlg.h"


extern CString LogInUserName;
// CAddIPDlg 对话框

IMPLEMENT_DYNAMIC(CAddIPDlg, CDialogEx)

CAddIPDlg::CAddIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddIPDlg::IDD, pParent)
{

}

CAddIPDlg::~CAddIPDlg()
{
}

void CAddIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDIP_IPADDRESS, m_addip_address);
}


BEGIN_MESSAGE_MAP(CAddIPDlg, CDialogEx)
	ON_BN_CLICKED(ID_SAVEIP_BUTTON, &CAddIPDlg::OnBnClickedSaveipButton)
END_MESSAGE_MAP()


// CAddIPDlg 消息处理程序


BOOL CAddIPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_addip_address.SetAddress(255,255,255,1);

	if(status == 0)//添加IP
	{
		GetDlgItem(IDC_EDITIP_STATIC)->SetWindowText("输入需要添加IP：");
	}
	else if(status == 1)
	{
		GetDlgItem(IDC_EDITIP_STATIC)->SetWindowText("输入需要删除IP：");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAddIPDlg::OnBnClickedSaveipButton()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret;
	BYTE IP[4];
	CFile file;
	CString readerrlog;

	m_addip_address.GetAddress(IP[0],IP[1],IP[2],IP[3]);
	//uart_printf("%d.%d.%d.%d\r\n",IP[0],IP[1],IP[2],IP[3]);
	
	//MessageBox( _T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix) , TEXT(TiShi) ,MB_OK);
	if(file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite))
	{
		if(status == 0)//添加IP
		{
			ret = AddCommonFile(file,IP);
			if(ret < 0)
			{
				readerrlog.Format("%s [%s]",ReadFileErr, ErrCord2004); 
				MessageBox( _T(readerrlog) , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 1)
			{
				MessageBox( _T("添加IP超过最大储存值") , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 2)
			{
				MessageBox( _T("IP地址已存在，无需重复添加") , TEXT(TiShi) ,MB_OK);
			}
		}
		else if(status == 1)//删除IP
		{
			ret = DelCommonFile(file,IP);
			if(ret < 0)
			{
				readerrlog.Format("%s [%s]",ReadFileErr, ErrCord2004); 
				MessageBox( _T(readerrlog) , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 1)
			{
				MessageBox( _T("没有存储IP数据") , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 2)
			{
				MessageBox( _T("未发现要删除的IP数据") , TEXT(TiShi) ,MB_OK);
			}
		}

		if(ret == 0)
		{
			MessageBox( _T("操作成功！！！") , TEXT(TiShi) ,MB_OK);
		}
		file.Close();
	}
	else
	{
		MessageBox( _T(ReadFileErr) , TEXT(TiShi) ,MB_OK);
	}
	
	CDialogEx::OnOK();
}


/*
对公共存储的IP数据读取、添加和删除
此四个函数暂时全部定义为静态函数
*/
int CAddIPDlg::ReadCommonFile(CFile &file,int *num,BYTE *IP)//传入的指针长度要够，防止溢出
{
	int readlen;
	BYTE ReadIP[COMMONDATALEN];

	*num = 0;
	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	
	if((readlen % 4) != 0)//文件异常
	{
		return -1;
	}
	if(readlen == 0)//未存储
	{
		return 0;
	}

	do
	{
		readlen -= 4;
		if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//全部为0，未存储，跳过遍历
			continue;
		else //有IP
		{
			memcpy(IP + (*num)*4,ReadIP + readlen,4);
			*num += 1;
		}
	}
	while(0 != readlen);

	return 0;
}
int CAddIPDlg::AddCommonFile(CFile &file,BYTE *IP)
{
	int readlen;
	BYTE ReadIP[COMMONDATALEN];

	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	if(readlen == 0)//第一次添加
	{
		file.Write(IP,4);
		return 0;
	}
	else if((readlen < COMMONDATALEN)&&(readlen > 0))//第二次之后添加
	{
		if((readlen % 4) != 0)//文件异常
		{
			return -1;
		}
		file.Seek(COMMONDATAADD + readlen,CFile::begin);
		file.Write(IP,4);
		return 0;
	}
	else if(readlen == COMMONDATALEN)
	{
		readlen = 0;
		do
		{
			if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//全部为0，即退出，未存储满
				break;
			else if((ReadIP[readlen] == IP[0])&&(ReadIP[readlen + 1] == IP[1])&&
				(ReadIP[readlen + 2] == IP[2])&&(ReadIP[readlen + 3] == IP[3]))//防重复
			{
				return 2;
			}
			readlen += 4;
		}
		while(COMMONDATALEN != readlen);

		if(COMMONDATALEN == readlen)//存储满了
		{
			return 1;
		}

		file.Seek(COMMONDATAADD + readlen,CFile::begin);
		file.Write(IP,4);
		return 0;
	}
	else if((readlen < 0)||(readlen > COMMONDATALEN))
	{
		return -2;
	}
	return 0;
}
int CAddIPDlg::DelCommonFile(CFile &file,BYTE *IP)// 0成功，1失败
{
	int readlen;
	BYTE ReadIP[COMMONDATALEN];

	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	
	if((readlen % 4) != 0)//文件异常
	{
		return -1;
	}
	if(readlen == 0)
	{
		return 1;
	}
	do
	{
		readlen -= 4;
		if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//全部为0，未存储，跳过遍历
			continue;
		else if((ReadIP[readlen] == IP[0])&&(ReadIP[readlen + 1] == IP[1])&&
			(ReadIP[readlen + 2] == IP[2])&&(ReadIP[readlen + 3] == IP[3]))//完全相同
		{
			memset(ReadIP + readlen,0,4);
			file.Seek(COMMONDATAADD + readlen,CFile::begin);
			file.Write(ReadIP + readlen,4);
			return 0;
		}
	}
	while(0 != readlen);

	if(readlen == 0)
	{
		return 2;
	}
	return 0;
}