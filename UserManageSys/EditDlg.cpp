// EditDlg.cpp : ʵ���ļ�
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

// CEditDlg �Ի���

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


// CEditDlg ��Ϣ�������


BOOL CEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ListAddLine(4);

	ListAddRow(0,1,"10.12.13.14",8080,"20180930");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEditDlg::ListAddLine(int line)//ΪList�ؼ������
{
	CRect rect;
	m_edit_list.GetClientRect(&rect);//��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	m_edit_list.SetExtendedStyle(m_edit_list.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����

	m_edit_list.InsertColumn(1,_T("ID"),LVCFMT_CENTER,rect.Width()/line);//Ϊ�б���ͼ�ؼ������
	m_edit_list.InsertColumn(2,_T("IP"),LVCFMT_CENTER,rect.Width()/line);
	m_edit_list.InsertColumn(3,_T("�˿�"),LVCFMT_CENTER,rect.Width()/line);
	m_edit_list.InsertColumn(4,_T("����ʱ��"),LVCFMT_CENTER,rect.Width()/line);
}

/*
	row:����ĵڼ���
	ID���û�ID
	IP���û�IP
	port���û��˿�
*/
void CEditDlg::ListAddRow(int row,int ID,CString IP,int port,CString Time)//ΪList�ؼ������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    
	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
  
	CMenu menu;
	menu.LoadMenu(IDR_MENU);
	CMenu* popup = menu.GetSubMenu(0);

	if(pNMListView->iItem != -1)
	{
		
	}
	else
	{
		popup->EnableMenuItem(ID_1,MF_BYCOMMAND|MF_ENABLED);  //�����½��˵���ʹ��
		popup->EnableMenuItem(ID_2,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //������༭�˵���ʹ��
		popup->EnableMenuItem(ID_3,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //������ɾ���˵���ʹ��
		popup->EnableMenuItem(ID_4,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //������˵���ʹ��
		popup->EnableMenuItem(ID_5,MF_BYCOMMAND|MF_ENABLED);  //�������IP�˵���ʹ��
	}
	ASSERT( popup != NULL );
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}


void CEditDlg::On1()//�½��û�
{
	// TODO: �ڴ���������������
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 1;//��ʾ�½��û�
	edituserdlg.DoModal();
}


void CEditDlg::On2()//����Ҫ��ת
{
	// TODO: �ڴ���������������
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 2;//��ʾɾ���û�
	edituserdlg.DoModal();
}


void CEditDlg::On3()
{
	// TODO: �ڴ���������������
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 3;//��ʾ�༭�û�
	edituserdlg.DoModal();
}


void CEditDlg::On4()
{
	// TODO: �ڴ���������������
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 4;//��ʾ��ϸ��Ϣ
	edituserdlg.DoModal();
}


void CEditDlg::On5()//���IP����
{
	// TODO: �ڴ���������������
	CAddIPDlg addipdlg;
	addipdlg.status = 0;
	addipdlg.DoModal();
}


void CEditDlg::On6()//ɾ��ip����
{
	// TODO: �ڴ���������������
	CAddIPDlg addipdlg;
	addipdlg.status = 1;
	addipdlg.DoModal();
}
