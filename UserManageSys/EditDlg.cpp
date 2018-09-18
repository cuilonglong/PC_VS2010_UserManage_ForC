// EditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "EditDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "Debug.h"
#include "UserInfo.h"
#include "EditUserDlg.h"
#include "AddIPDlg.h"
#include "UserManageSysDlg.h"


extern CString LogInUserName;
// CEditDlg 对话框

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
{

}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LIST, m_edit_list);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_EDIT_LIST, &CEditDlg::OnNMRClickEditList)
	ON_COMMAND(ID_1, &CEditDlg::On1)
	ON_COMMAND(ID_2, &CEditDlg::On2)
	ON_COMMAND(ID_3, &CEditDlg::On3)
	ON_COMMAND(ID_4, &CEditDlg::On4)
	ON_COMMAND(ID_5, &CEditDlg::On5)
	ON_COMMAND(ID_6, &CEditDlg::On6)
END_MESSAGE_MAP()


// CEditDlg 消息处理程序


BOOL CEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ListAddLine(4);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEditDlg::ShowEditListView()//初始化链表控件
{
	CFile file;
	CString password;
	CUserInfo userinfo;
	int errnum,time[4],usernum,ret,num;

	m_edit_list.DeleteAllItems();
	//ListAddRow(0,1111,"10.12.13.14",8080,"20180930");
	if(LogInUserName == "")//未登录
	{
		return;
	}

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*文件尾缀*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//获取已经存储的用户个数
	if(ret != 0){
		::MessageBox( NULL,_T("初始化获取用户数据失败！") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum == 0)//无用户数据
	{
		goto Err;
	}
	int index = 0;
	num = 0;
	do
	{
		ret = CEditUserDlg::ReadUserInfo(file,userinfo,num++);
		if(ret != 0)
			continue;

		ListAddRow(index,userinfo.userid,userinfo.userip,userinfo.port,userinfo.endtime);
		index++;

		if(index == usernum)//遍历完成
			break;
	}
	while(index <= USERNUMMAX);

Err:
	file.Close();
	return;
}


void CEditDlg::ListAddLine(int line)//为List控件添加列
{
	CRect rect;
	m_edit_list.GetClientRect(&rect);//获取编程语言列表视图控件的位置和大小
	m_edit_list.SetExtendedStyle(m_edit_list.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//为列表视图控件添加全行选中和栅格风格

	m_edit_list.InsertColumn(1,_T("ID"),LVCFMT_CENTER,rect.Width()/line);//为列表视图控件添加列
	m_edit_list.InsertColumn(2,_T("IP"),LVCFMT_CENTER,rect.Width()/line);
	m_edit_list.InsertColumn(3,_T("端口"),LVCFMT_CENTER,rect.Width()/line);
	m_edit_list.InsertColumn(4,_T("到期时间"),LVCFMT_CENTER,rect.Width()/line);
}

/*
	row:插入的第几行
	ID：用户ID
	IP：用户IP
	port：用户端口
*/
void CEditDlg::ListAddRow(int row,int ID,CString IP,int port,CString Time)//为List控件添加行
{
	CString str;

	str.Format("%04d",ID);
	m_edit_list.InsertItem(row,str);

	m_edit_list.SetItemText(row,1,IP);

	str.Format("%04d",port);
	m_edit_list.SetItemText(row,2,str);

	m_edit_list.SetItemText(row,3,Time);
}


void CEditDlg::OnNMRClickEditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    
	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
  
	CMenu menu;
	menu.LoadMenu(IDR_MENU);
	CMenu* popup = menu.GetSubMenu(0);

	if(pNMListView->iItem != -1)//有选中
	{
		CString idnum = m_edit_list.GetItemText(pNMListView->iItem, 0);//获取选择的ID
		IDNUM = _ttoi(idnum);//转换成整数
	}
	else//未显示任何项
	{
		popup->EnableMenuItem(ID_1,MF_BYCOMMAND|MF_ENABLED);  //允许新建菜单项使用
		popup->EnableMenuItem(ID_2,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //不允许编辑菜单项使用
		popup->EnableMenuItem(ID_3,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //不允许删除菜单项使用
		popup->EnableMenuItem(ID_4,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //不允许菜单项使用
		popup->EnableMenuItem(ID_5,MF_BYCOMMAND|MF_ENABLED);  //允许添加IP菜单项使用
	}
	ASSERT( popup != NULL );
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}


void CEditDlg::On1()//新建用户
{
	// TODO: 在此添加命令处理程序代码
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 1;//表示新建用户
	edituserdlg.DoModal();
	ShowEditListView();
}


void CEditDlg::On2()//表示删除用户//不需要跳转
{
	// TODO: 在此添加命令处理程序代码
	int ret;
	CString log;
	log.Format("确认删除 ID:%04d 账户吗？",IDNUM);
	ret = MessageBox( _T(log) , TEXT(TiShi) ,MB_OKCANCEL);
	if(ret == IDCANCEL)
	{
		return;
	}
	ret = CEditUserDlg::DelUserInfo(IDNUM);
	if(ret == 0)
	{
		log.Format("删除 ID:%04d成功！",IDNUM);
		MessageBox( _T(log) , TEXT(TiShi) ,MB_OK);
	}
	else if(ret == 100)
	{
		log.Format("删除 ID:%04d失败！\r\nID用户不存在！",IDNUM);
		MessageBox( _T(log) , TEXT(TiShi) ,MB_OK);
	}
	else
	{
		log.Format("删除 ID:%04d失败！",IDNUM);
		MessageBox( _T(log) , TEXT(TiShi) ,MB_OK);
	}
	ShowEditListView();
}


void CEditDlg::On3()
{
	// TODO: 在此添加命令处理程序代码
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 3;//表示编辑用户
	edituserdlg.DoModal();
	ShowEditListView();
}


void CEditDlg::On4()
{
	// TODO: 在此添加命令处理程序代码
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 4;//表示详细信息
	edituserdlg.DoModal();
}


void CEditDlg::On5()//添加IP数据
{
	// TODO: 在此添加命令处理程序代码
	CAddIPDlg addipdlg;
	addipdlg.status = 0;
	addipdlg.DoModal();
}


void CEditDlg::On6()//删除ip数据
{
	// TODO: 在此添加命令处理程序代码
	CAddIPDlg addipdlg;
	addipdlg.status = 1;
	addipdlg.DoModal();
}
