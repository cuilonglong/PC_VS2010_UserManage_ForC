// PassWorldDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "PassWorldDlg.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "LoginDlg.h"

// CPassWorldDlg �Ի���

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


// CPassWorldDlg ��Ϣ�������


void CPassWorldDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_ACCOUNT_EDIT,account);
	GetDlgItemText(IDC_PASSWORLD_EDIT,password);

	CDialogEx::OnOK();
}


void CPassWorldDlg::OnStnClickedPasswordStatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	INT_PTR nRes;

	CLoginDlg logindlg;
	do
	{
		nRes = logindlg.DoModal();
		if(logindlg.status == 0)//ע����Ϣ��д�ɹ�
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_login_Font.CreateFont(-12,0,0,0,700,
	0,1,0,0,0,0,0,0,
	_T("���Ĳ���"));//�����»���

	//m_login_Font.CreatePointFont(150, _T("���Ĳ���"), NULL);
	m_passworld_static.SetFont(&m_login_Font,true);  

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CPassWorldDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_PASSWORD_STATIC)
	{
		pDC->SetTextColor(RGB(0, 0, 255));	// ������ɫ
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;

}


BOOL CPassWorldDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	//	 ::SetCursor(cursor);//��������ó�����
	//	 SetClassLong(this->GetSafeHwnd(),
	//		 GCL_HCURSOR,
	//		 (LONG)LoadCursor(NULL, IDC_HAND));//ʹ����ڸ���������˸
	// }
	// else
	// {
	//	 SetClassLong(this->GetSafeHwnd(),
	//		 GCL_HCURSOR,
	//		 (LONG)LoadCursor(NULL, IDC_ARROW));//����뿪������ָ�Ĭ�ϼ�ͷ��״
	// }
	CDialogEx::OnMouseMove(nFlags, point);
}




BOOL CPassWorldDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
	{
 
		CRect rect;
		GetDlgItem(IDC_PASSWORD_STATIC)->GetWindowRect(&rect);		//��ȡ�ؼ����� �������Ļ���Ͻ�
 
		CPoint  pt;
		GetCursorPos(&pt);		//��ȡ�������Ļ�����е�λ�� 
 
		if (rect.PtInRect(pt))
		{
			//��control������
			 HCURSOR cursor = LoadCursor(NULL,IDC_HAND);
			 ::SetCursor(cursor);//��������ó�����
			 SetClassLong(this->GetSafeHwnd(),
				 GCL_HCURSOR,
				 (LONG)LoadCursor(NULL, IDC_HAND));//ʹ����ڸ���������˸
 
		}
		else
		{
			//����control�����ڣ���ʾ��Ϣ������ͼƬ�� 
			SetClassLong(this->GetSafeHwnd(),
			GCL_HCURSOR,
			(LONG)LoadCursor(NULL, IDC_ARROW));//����뿪������ָ�Ĭ�ϼ�ͷ��״
 
		}   
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
