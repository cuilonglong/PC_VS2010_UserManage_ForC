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
#include "UserManageSysDlg.h"


extern CString LogInUserName;
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEditDlg::ShowEditListView()//��ʼ������ؼ�
{
	CFile file;
	CString password;
	CUserInfo userinfo;
	int errnum,time[4],usernum,ret,num;

	m_edit_list.DeleteAllItems();
	//ListAddRow(0,1111,"10.12.13.14",8080,"20180930");
	if(LogInUserName == "")//δ��¼
	{
		return;
	}

	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(ret != 0){
		::MessageBox( NULL,_T("��ʼ����ȡ�û�����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		goto Err;
	}
	if(usernum == 0)//���û�����
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

		if(index == usernum)//�������
			break;
	}
	while(index <= USERNUMMAX);

Err:
	file.Close();
	return;
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

	if(pNMListView->iItem != -1)//��ѡ��
	{
		CString idnum = m_edit_list.GetItemText(pNMListView->iItem, 0);//��ȡѡ���ID
		IDNUM = _ttoi(idnum);//ת��������
	}
	else//δ��ʾ�κ���
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
	ShowEditListView();
}


void CEditDlg::On2()//��ʾɾ���û�//����Ҫ��ת
{
	// TODO: �ڴ���������������
	int ret;
	CString log;
	log.Format("ȷ��ɾ�� ID:%04d �˻���",IDNUM);
	ret = MessageBox( _T(log) , TEXT(TiShi) ,MB_OKCANCEL);
	if(ret == IDCANCEL)
	{
		return;
	}
	ret = CEditUserDlg::DelUserInfo(IDNUM);
	if(ret == 0)
	{
		log.Format("ɾ�� ID:%04d�ɹ���",IDNUM);
		MessageBox( _T(log) , TEXT(TiShi) ,MB_OK);
	}
	else if(ret == 100)
	{
		log.Format("ɾ�� ID:%04dʧ�ܣ�\r\nID�û������ڣ�",IDNUM);
		MessageBox( _T(log) , TEXT(TiShi) ,MB_OK);
	}
	else
	{
		log.Format("ɾ�� ID:%04dʧ�ܣ�",IDNUM);
		MessageBox( _T(log) , TEXT(TiShi) ,MB_OK);
	}
	ShowEditListView();
}


void CEditDlg::On3()
{
	// TODO: �ڴ���������������
	CEditUserDlg edituserdlg;
	edituserdlg.Buttonstatus = 3;//��ʾ�༭�û�
	edituserdlg.DoModal();
	ShowEditListView();
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
