
// UserManageSysDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "UserManageSysDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "LoginDlg.h"
#include "UserInfo.h"
#include "AES.h"
#include "Debug.h"
#include "GetNetTime.h"
#include "EditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int UserStatus = 0;//软件的登陆状态;
CString LogInUserName = "";//登陆用户的名字

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUserManageSysDlg 对话框
CUserManageSysDlg::CUserManageSysDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserManageSysDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserManageSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TAB,m_tab); 
	DDX_Control(pDX, IDC_MAIN_STATIC, m_main_static);
}

BEGIN_MESSAGE_MAP(CUserManageSysDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CUserManageSysDlg::OnTcnSelchangeTab)
	ON_STN_CLICKED(IDC_MAIN_STATIC, &CUserManageSysDlg::OnStnClickedMainStatic)
END_MESSAGE_MAP()


// CUserManageSysDlg 消息处理程序

BOOL CUserManageSysDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	TabViewInit();//tab控件初始化

	m_status_Font.CreateFont(-12,0,0,0,700,
	0,1,0,0,0,0,0,0,
	_T("华文彩云"));//增加下划线
	m_main_static.SetFont(&m_status_Font,true); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void  CUserManageSysDlg::TabViewInit()
{
	CRect tabRect;   // 标签控件客户区的位置和大小   

	m_tab.InsertItem(0, _T("查找"));// 插入第一个标签“查找”
	m_tab.InsertItem(1, _T("编辑"));

	m_findDlg.Create(IDD_FIND_DIALOG,&m_tab);
	m_editDlg.Create(IDD_EDIT_DIALOG,&m_tab);

	ShowTabViewForSel(0);
}

void CUserManageSysDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUserManageSysDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUserManageSysDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUserManageSysDlg::OnSize(UINT nType, int cx, int cy)
{
	
	CDialogEx::OnSize(nType, cx, cy);

}


void CUserManageSysDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	 // 将最小跟踪与最大跟踪设置成相同的值使窗口大小不能改变
    lpMMI->ptMinTrackSize.x = WindowSizeWide; // 设定最小跟踪宽度
    lpMMI->ptMinTrackSize.y = WindowSizeHigh; // 设定最小跟踪高度
    lpMMI->ptMaxTrackSize.x = WindowSizeWide; // 设定最大跟踪宽度
    lpMMI->ptMaxTrackSize.y = WindowSizeHigh; // 设定最大跟踪高度

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CUserManageSysDlg::ShowTabViewForSel(int sel)
{
	CRect tabRect;    // 标签控件客户区的Rect   
   
    // 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
    m_tab.GetClientRect(&tabRect);   
    tabRect.left += 1;   
    tabRect.right -= 1;   
    tabRect.top += 22;   
    tabRect.bottom -= 1;   

	switch (sel)   
    {   
    case 0:   //显示查找界面
        m_findDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
        m_editDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		m_findDlg.m_find_list.DeleteAllItems();

        break;   
    case 1:   //显示编辑界面
		m_findDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		m_editDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);  
		
		m_editDlg.ShowEditListView();
        break;   
    default:   
        break;   
    }
}

int CUserManageSysDlg::CheckPassWorld()//校验密码
{
	INT_PTR nRes;//用于保存DoModal函数的返回值
	CPassWorldDlg passworlddlg;//构造对话框类的实例

	nRes = passworlddlg.DoModal();//弹出输入密码会话框
	if(IDCANCEL == nRes)//点击取消
	{
		//MessageBox( TEXT(ClickCancel) , TEXT(TiShi) ,MB_OK);
		return 1;
	}
	else if(IDOK == nRes)//点击确认
	{
		//MessageBox( TEXT(ClickOk) , TEXT(TiShi) ,MB_OK);
		CFile file;
		if((passworlddlg.password == "")||(passworlddlg.account == ""))//判断输入账号不为空
		{
			MessageBox( _T(NIUserInfo) , TEXT(TiShi) ,MB_OK);
			return 2;
		}

		if(file.Open(_T(GetFilePath()+passworlddlg.account+ FileSuffix/*文件尾缀*/), CFile::modeReadWrite))//账户文件存在
		{
			CString password;
			SYSTEMTIME st;
			int errnum,time[4],usernum;

			nRes = ExplainFileHead(file,passworlddlg.account,password,errnum,time,usernum);
			if(nRes != 0)
			{
				file.Close();//关闭文件
				return nRes;
			}
			else if((GetNetTime::getnettime(st) != 0)&&(errnum >= 2))//没有网络且密码输入超过三次
			{
				MessageBox( _T("账户已锁定，未联网允许输入三次错误密码") , TEXT(TiShi) ,MB_OK);
				return -2;
			}
			else if(errnum >= 4)
			{
				if(CheckTimeOut24hour(time,st) == 0)//24h到了
				{
					errnum = 0;
					EditFileHead(file,errnum,time);
				}
				else//锁定
				{
					MessageBox( _T(UserLock) , TEXT(TiShi) ,MB_OK);
					return -4;
				}
			}
			uart_printf("%04d %02d %02d %02d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour);

			if(passworlddlg.password == password)
			{
				MessageBox( _T(WecomeUser) , TEXT(TiShi) ,MB_OK);
				UserStatus = 1;//已登录
				LogInUserName = passworlddlg.account;

				CString statusstring;
				statusstring.Format("用户：%s",passworlddlg.account);
				GetDlgItem(IDC_MAIN_STATIC)->SetWindowText(statusstring);
				errnum = 0;
				EditFileHead(file,errnum,time);//输对之后重置次数
				file.Close();//关闭文件
				return 0;
			}
			else
			{
				CString hintlog;
				hintlog.Format("%s %d次\r\n%s",PasswordErr,(errnum+1),"电脑未联网24允许输入错误3次，联网允许24小时输入5次");
				MessageBox( _T(hintlog) , TEXT(TiShi) ,MB_OK);
				if(errnum == 0)//第一次输入错误
				{
					//uart_printf("%02x %02x %02x %02x",st.wYear,st.wMonth,st.wDay,st.wHour);
					time[0] = st.wYear;time[1] = st.wMonth;time[2] = st.wDay;time[3] = st.wHour;
					errnum++;
					EditFileHead(file,errnum,time);
					return -3;
				}
				else
				{
					//MessageBox( _T("多次输入错误密码，五次将被锁定24小时") , TEXT(TiShi) ,MB_OK);
					errnum++;
					EditFileHead(file,errnum,time);
					return -1;
				}
			}
		}
		else//第一次打开
		{
			return LoginUserAccount();//注册账号
		}
	}
	else//点击别的按钮
	{
		//MessageBox( TEXT(ClickOther) , TEXT(TiShi) ,MB_OK);
		return -2;
	}
}


/*
校验是否到达24小时
return 0：已到
return 1：未到

time:记录的时间
st：//获取的网络时间
*/
int CUserManageSysDlg::CheckTimeOut24hour(int *time ,SYSTEMTIME st)
{
	CTime t1( time[0], time[1], time[2], time[3], 30, 0 );
	CTime t2( st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	CTimeSpan span = t2 - t1;
	if(span.GetDays() >= 1)
		return 0;
	else
		return 1;
}


/*
errnum:传入当前错误的次数、0表示输入正确，重置;1代表第一次输错;2~5代表输错次数(传入-1不处理错误次数和时间)
time:当errnum为0时需要传入时间
usernum:需要更新的用户个数
*/

int CUserManageSysDlg::EditFileHead(CFile &file,int &errnum,int* time,int usernum)//修改相关密码输错次数数据
{
	CString readerrlog;
	DataHeard datahead;
	int readlen = 0,seeklen = 0;
	uchar headkcv[USERINFOHEADLEN-16];
	uchar dataaccount[32],datapassword[32];

	file.SeekToBegin();
	readlen = file.Read((int *)&(datahead.headlen), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.headlen),sizeof(int));
	seeklen += readlen;//读取head

	readlen = file.Read((int *)&(datahead.AccountLen), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.AccountLen),sizeof(int));
	seeklen += readlen;//读取账户长度

	readlen = file.Read(dataaccount,32);
	if(readlen != 32)
		goto ERR;
	memcpy(headkcv+seeklen,dataaccount,32);
	seeklen += 32;//读取账户数据

	readlen = file.Read((int *)&(datahead.PassworldLen), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.PassworldLen),sizeof(int));
	seeklen += readlen;//读取密码长度

	readlen = file.Read(datapassword,32);
	if(readlen != 32)
		goto ERR;
	memcpy(headkcv+seeklen,datapassword,32);
	seeklen += 32;//读取密码数据

	if(errnum == -1)
	{
		readlen = file.Read((int *)&(datahead.InputErrNum), sizeof(int));
		if(readlen != sizeof(int))
			goto ERR;
	}else
	{
		datahead.InputErrNum = (errnum);
		file.Write((int *)&(datahead.InputErrNum),sizeof(int));
	}
	memcpy(headkcv+seeklen,(uchar *)&(datahead.InputErrNum),sizeof(int));
	seeklen += sizeof(int);//读取InputErrNum

	if(usernum == -1)
	{
	readlen = file.Read((int *)&(datahead.UserNum), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	}
	else
	{
		datahead.UserNum = (usernum);
		file.Write((int *)&(datahead.UserNum),sizeof(int));
	}
	memcpy(headkcv+seeklen,(uchar *)&(datahead.UserNum),sizeof(int));
	seeklen += sizeof(int);//读取UserNum

	if(errnum == 0)//重置时间
	{
		memset((uchar *)&(datahead.Time),0x00,4*sizeof(int));
		file.Write((int *)&(datahead.Time),4*sizeof(int));
	}
	else if(errnum == 1)
	{
		memcpy((uchar *)&(datahead.Time),time,4*sizeof(int));
		file.Write((int *)&(datahead.Time),4*sizeof(int));
	}
	else
	{
		readlen = file.Read((int *)datahead.Time, 4*sizeof(int));
		if(readlen != 4*sizeof(int))
			goto ERR;
	}

	memcpy(headkcv+seeklen,(uchar *)&(datahead.Time),4*sizeof(int));
	seeklen += 4*sizeof(int);//读取Time

	AES::AES_GetFeature((uchar *)headkcv,seeklen,(uchar *)datahead.HCV);

	//printf_format((char *)headkcv,seeklen);

	file.Write((uchar *)datahead.HCV,16);
	goto CORR;

ERR:
	readerrlog.Format("%s [%s]",ReadFileErr, ErrCord2002); 
	::MessageBox(NULL, _T(readerrlog) , TEXT(TiShi) ,MB_OK);
	//file.Close();//关闭文件
	return -1;

CORR:
	//file.Close();//关闭文件
	return 0;
}

/*
	解析数据头文件
	password：返回密码
	errnum：输错次数
	time:第一次输入错误的时间
	usernum:已存储的用户个数
*/
int CUserManageSysDlg::ExplainFileHead(CFile &file,CString account,CString &password,int &errnum,int *time,int &usernum)//解析文件头（用户信息）
{
	CString readerrlog;
	DataHeard datahead;
	int seeklen = 0,readlen = 0;
	uchar kcv[16],key[16],headkcv[USERINFOHEADLEN-16]/*计算kcv临时缓冲区*/;
	uchar dataaccount[32],datapassword[32];

	AES::AES_GetFeature((uchar *)(account + account).GetBuffer(),(account + account).GetLength(),key);

	file.SeekToBegin();
	readlen = file.Read((int *)&(datahead.headlen), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.headlen),sizeof(int));
	seeklen += readlen;//读取head

	readlen = file.Read((int *)&(datahead.AccountLen), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.AccountLen),sizeof(int));
	seeklen += readlen;//读取账户长度

	readlen = file.Read(dataaccount,32);
	if(readlen != 32)
		goto ERR;
	memcpy(headkcv+seeklen,dataaccount,32);
	seeklen += 32;//读取账户数据

	readlen = file.Read((int *)&(datahead.PassworldLen), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.PassworldLen),sizeof(int));
	seeklen += readlen;//读取密码长度

	readlen = file.Read(datapassword,32);
	if(readlen != 32)
		goto ERR;
	memcpy(headkcv+seeklen,datapassword,32);
	seeklen += 32;//读取密码数据


	readlen = file.Read((int *)&(datahead.InputErrNum), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.InputErrNum),sizeof(int));
	seeklen += readlen;//读取InputErrNum


	readlen = file.Read((int *)&(datahead.UserNum), sizeof(int));
	if(readlen != sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.UserNum),sizeof(int));
	seeklen += readlen;//读取UserNum

	readlen = file.Read((int *)datahead.Time, 4*sizeof(int));
	if(readlen != 4*sizeof(int))
		goto ERR;
	memcpy(headkcv+seeklen,(uchar *)&(datahead.Time),4*sizeof(int));
	seeklen += readlen;//读取Time

	readlen = file.Read((uchar *)datahead.HCV, 16);
	if(readlen != 16)
		goto ERR;

	AES::AES_GetFeature((uchar *)headkcv,seeklen,(uchar *)kcv);
	//printf_format((char *)headkcv,seeklen);
	if(memcmp(kcv,datahead.HCV,16))//校验HCV
	{
		
		readerrlog.Format("%s [%s]",ReadFileErr, ErrCord2003); 
		::MessageBox( NULL,_T(readerrlog) , TEXT(TiShi) ,MB_OK);
		goto ERR1;
	}

	AES::AES_Decryption(key,16,datapassword,32);
	datahead.Passworld.Format("%s",datapassword);

	AES::AES_Decryption(key,16,dataaccount,32);//解密数据
	datahead.Account.Format("%s",dataaccount);
	goto CORR;

ERR:
	readerrlog.Format("%s [%s]",ReadFileErr, ErrCord2002); 
	::MessageBox( NULL,_T(readerrlog) , TEXT(TiShi) ,MB_OK);
ERR1:
	//file.Close();//关闭文件
	return -1;

CORR:
	password = datahead.Passworld;
	errnum = datahead.InputErrNum;
	usernum = datahead.UserNum;
	memcpy(time,datahead.Time,4*sizeof(int));
	//file.Close();//关闭文件
	return 0;
}


int CUserManageSysDlg::LoginUserAccount()//注册账户
{
	INT_PTR nRes;
	
	nRes = MessageBox( TEXT(UserNLogin) , TEXT(TiShi) ,MB_YESNO);
	if(IDYES == nRes)//需要重新注册
	{
		CLoginDlg logindlg;
		do
		{
			nRes = logindlg.DoModal();
			if(logindlg.status == 0)//注册信息填写成功
			{
				MessageBox( _T(LoginSuccess) , TEXT(TiShi) ,MB_OK);
				break;
			}
			else if(nRes == IDCANCEL)
			{
				return -1;
			}
		}while(1);

	}
	else if(IDNO == nRes)
	{
		return 3;
	}
	return 4;
}


void CUserManageSysDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if(m_tab.GetCurSel() != 0)//除第一个界面都需验证密码
	{
		if(UserStatus != 1)//未登录
		{
			if(CheckPassWorld() != 0)//验证密码错误
			{
				m_tab.SetCurSel(0);
			}
		}	
	}
	ShowTabViewForSel(m_tab.GetCurSel());
	return;
}


/*
获取软件所在路径
*/
CString CUserManageSysDlg::GetFilePath()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	CString strPath;
	CString strFullName = AfxGetApp()->m_pszHelpFilePath;

	_splitpath(strFullName, drive, dir, NULL,NULL);
	strPath.Format("%s%s", drive, dir);

	return strPath;
}


void CUserManageSysDlg::OnStnClickedMainStatic()//单击登陆
{
	// TODO: 在此添加控件通知处理程序代码
	int ret;
	
	if(UserStatus == 0)
	{
		CheckPassWorld();
		return;
	}
	else
	{
		ret= MessageBox( _T(ExitUser) , TEXT(TiShi) ,MB_OKCANCEL);
		if(ret == IDOK)
		{
			UserStatus = 0;//置换成未登录
			//LogInUserName = "";
			GetDlgItem(IDC_MAIN_STATIC)->SetWindowText("未登录");
			m_tab.SetCurSel(0);
			ShowTabViewForSel(m_tab.GetCurSel());//切回第一个界面
		}
		else
		{
			return;
		}
	}
}


BOOL CUserManageSysDlg::PreTranslateMessage(MSG* pMsg)//截取鼠标的信息，变箭头为手
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
	{
 
		CRect rect;
		GetDlgItem(IDC_MAIN_STATIC)->GetWindowRect(&rect);		//获取控件坐标 相对于屏幕左上角
 
		CPoint  pt;
		GetCursorPos(&pt);		//获取光标在屏幕坐标中的位置 
 
		if (rect.PtInRect(pt))
		{
			//在control区域内
			 HCURSOR cursor = LoadCursor(NULL,IDC_HAND);
			 ::SetCursor(cursor);//将光标设置成手势
			 SetClassLong(this->GetSafeHwnd(),
				 GCL_HCURSOR,
				 (LONG)LoadCursor(NULL, IDC_HAND));//使光标在该区域不再闪烁
 
		}
		else
		{
			//不在control区域内，提示信息，更换图片等 
			SetClassLong(this->GetSafeHwnd(),
			GCL_HCURSOR,
			(LONG)LoadCursor(NULL, IDC_ARROW));//光标离开该区域恢复默认箭头形状
 
		}   
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
