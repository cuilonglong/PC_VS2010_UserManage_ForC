// PassWorldDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "PassWorldDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "LoginDlg.h"

// CPassWorldDlg 对话框

IMPLEMENT_DYNAMIC(CPassWorldDlg, CDialogEx)

CPassWorldDlg::CPassWorldDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassWorldDlg::IDD, pParent)
{

}

CPassWorldDlg::~CPassWorldDlg()
{
}

void CPassWorldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PASSWORD_STATIC, m_passworld_static);
	DDX_Control(pDX, IDC_PASSWORD1_STATIC, m_passworld_static1);
}


BEGIN_MESSAGE_MAP(CPassWorldDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPassWorldDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_PASSWORD_STATIC, &CPassWorldDlg::OnStnClickedPasswordStatic)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CPassWorldDlg 消息处理程序


void CPassWorldDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_ACCOUNT_EDIT,account);
	GetDlgItemText(IDC_PASSWORLD_EDIT,password);

	CDialogEx::OnOK();
}


void CPassWorldDlg::OnStnClickedPasswordStatic()
{
	// TODO: 在此添加控件通知处理程序代码

	INT_PTR nRes;

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
			return;
		}
	}while(1);

	return;
}


BOOL CPassWorldDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_login_Font.CreateFont(-12,0,0,0,700,
	0,1,0,0,0,0,0,0,
	_T("华文彩云"));//增加下划线

	//m_login_Font.CreatePointFont(150, _T("华文彩云"), NULL);
	m_passworld_static.SetFont(&m_login_Font,true);  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CPassWorldDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID()==IDC_PASSWORD_STATIC)
	{
		pDC->SetTextColor(RGB(0, 0, 255));	// 设置颜色
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;

}


BOOL CPassWorldDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CPassWorldDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseHover(nFlags, point);
}


void CPassWorldDlg::OnMouseLeave()
{
	CDialogEx::OnMouseLeave();
}


void CPassWorldDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	//CRect rect;
	//m_passworld_static.GetWindowRect(&rect);
	//CPoint  pt;
	//GetCursorPos(&pt);

	//if (rect.PtInRect(pt))
	// {
	//	 HCURSOR cursor = LoadCursor(NULL,IDC_HAND);
	//	 ::SetCursor(cursor);//将光标设置成手势
	//	 SetClassLong(this->GetSafeHwnd(),
	//		 GCL_HCURSOR,
	//		 (LONG)LoadCursor(NULL, IDC_HAND));//使光标在该区域不再闪烁
	// }
	// else
	// {
	//	 SetClassLong(this->GetSafeHwnd(),
	//		 GCL_HCURSOR,
	//		 (LONG)LoadCursor(NULL, IDC_ARROW));//光标离开该区域恢复默认箭头形状
	// }
	CDialogEx::OnMouseMove(nFlags, point);
}




BOOL CPassWorldDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
	{
 
		CRect rect;
		GetDlgItem(IDC_PASSWORD_STATIC)->GetWindowRect(&rect);		//获取控件坐标 相对于屏幕左上角
 
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
