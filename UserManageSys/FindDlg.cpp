// FindDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "FindDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "AES.h"
#include "Debug.h"
#include "UserInfo.h"
#include "UserManageSysDlg.h"
#include "EditUserDlg.h"
#include "AddIPDlg.h"

extern int UserStatus;
extern CString LogInUserName;
// CFindDlg 对话框

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
	DDX_Control(pDX, IDC_FIND_EDIT_LIST, m_find_list);
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CFindDlg::OnBnClickedFindButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFindDlg 消息处理程序


BOOL CFindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ComboViewInit();
	ListAddLine(4);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFindDlg::ComboViewInit()//Combo控件初始化界面
{
	m_combo.AddString(_T("端口"));//sel = 0
	m_combo.AddString(_T("未续费"));
	m_combo.AddString(_T("IP"));
	m_combo.AddString(_T("QQ"));//sel = 3

	m_combo.SetCurSel(0);
}


/*
	row:插入的第几行
	port：用户端口
	status：用户续费状态
	IP：用户IP
	QQ:用户QQ
*/
void CFindDlg::ListAddRow(int row,int port,int status,CString IP,CString QQ)//为List控件添加行
{
	CString str;

	str.Format("%04d",port);
	int nIndex = m_find_list.InsertItem(row,str);

	switch (status){
	case 0:
		str = "使用中";
		break;
	case 1:
		str = "已关闭";
		break;
	case 2:
		str = "未续费";
		break;
	case 3:
		str = "试用中";
		break;
	default:
		str = "未知";
			break;
	}
	m_find_list.SetItemText(nIndex,1,str);

	m_find_list.SetItemText(nIndex,2,IP);
	m_find_list.SetItemText(nIndex,3,QQ);
}

void CFindDlg::ListAddLine(int line)//为List控件添加列
{
	CRect rect;
	m_find_list.GetClientRect(&rect);//获取编程语言列表视图控件的位置和大小
	m_find_list.SetExtendedStyle(m_find_list.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//为列表视图控件添加全行选中和栅格风格

	m_find_list.InsertColumn(1,_T("端口"),LVCFMT_CENTER,rect.Width()/line);//为列表视图控件添加列
	m_find_list.InsertColumn(2,_T("续费状态"),LVCFMT_CENTER,rect.Width()/line);
	m_find_list.InsertColumn(3,_T("IP"),LVCFMT_CENTER,rect.Width()/line);
	m_find_list.InsertColumn(4,_T("QQ"),LVCFMT_CENTER,rect.Width()/line);
}

void CFindDlg::OnBnClickedFindButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile file;
	CUserInfo userinfo;
	CString FindString;
	int num,ret,i,port;
	LONGLONG seek[USERNUMMAX];
	BYTE userdata[USERINFOLEN];

	GetDlgItemText(IDC_FIND_EDIT,FindString);
	if((FindString == "")&&(m_combo.GetCurSel() !=1))
	{
		MessageBox( TEXT(NInFindInfo) , TEXT(TiShi) ,MB_OK);
		return;
	}
	if(UserStatus == 0)//未登录
	{
		MessageBox( TEXT("未登录账户，清先登录账号！") , TEXT(TiShi) ,MB_OK);
		return;
	}

	m_find_list.DeleteAllItems();//每次查询先清除
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	switch (m_combo.GetCurSel())
	{
		case 0://端口
			port = _ttoi(FindString);
			if((port > 9999)||(port <= 0))
			{
				MessageBox( TEXT("请输入合法的端口！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			ret = CEditUserDlg::FindUserInfo(file,2,port,"",seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("未查询到该端口！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("查询用户端口错误！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//循环把查询结果显示
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//解析数据
				ListAddRow(i,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
			}
			while(i != num);
			break;

		case 1://续费状态
			ret = CEditUserDlg::FindUserInfo(file,4,0,FindString,seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("未查询到未续费用户！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("查询未续费用户错误！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//循环把查询结果显示
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//解析数据
				ListAddRow(i,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
			}
			while(i != num);
			break;

		case 2://IP
			ret = CAddIPDlg::GetIPSeek(FindString,&port,file);//获取ip的偏移port//名字将就用
			if(ret != 0)
			{
				MessageBox( TEXT("请输入合法的IP！\r\n例如：255.255.255.1") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			ret = CEditUserDlg::FindUserInfo(file,5,port,"",seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("未查询到该服务器！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("查询用户服务器错误！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//循环把查询结果显示
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//解析数据
				ListAddRow(i,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
			}
			while(i != num);
			break;

		case 3://QQ
			ret = CEditUserDlg::FindUserInfo(file,3,0,FindString,seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("未查询到该QQ用户！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("查询用户QQ错误！") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//循环把查询结果显示
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//解析数据
				ListAddRow(i,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
			}
			while(i != num);
			break;
		default:
			break;
	}
	//MessageBox( TEXT(NFUserInfo) , TEXT(TiShi) ,MB_OK);

	/*SetTimer(1, 1000, NULL); */
	return;
}


void CFindDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
