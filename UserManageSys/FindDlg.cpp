// FindDlg.cpp : ʵ���ļ�
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
#include "AddAmountDlg.h"

extern int UserStatus;
extern CString LogInUserName;
// CFindDlg �Ի���

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
	ON_NOTIFY(NM_DBLCLK, IDC_FIND_EDIT_LIST, &CFindDlg::OnNMDblclkFindEditList)
	ON_CBN_SELCHANGE(IDC_FIND_COMBO, &CFindDlg::OnCbnSelchangeFindCombo)
	ON_NOTIFY(NM_RCLICK, IDC_FIND_EDIT_LIST, &CFindDlg::OnNMRClickFindEditList)
	ON_COMMAND(ID_8, &CFindDlg::On8)
END_MESSAGE_MAP()


// CFindDlg ��Ϣ�������


BOOL CFindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ComboViewInit();
	ListAddLine(5);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFindDlg::ComboViewInit()//Combo�ؼ���ʼ������
{
	m_combo.AddString(_T("�˿�"));//sel = 0
	m_combo.AddString(_T("δ����"));
	m_combo.AddString(_T("IP"));
	m_combo.AddString(_T("QQ"));//sel = 3
	m_combo.AddString(_T("������"));
	m_combo.AddString(_T("�ѹر�"));
	m_combo.AddString(_T("ʹ����"));
	m_combo.SetCurSel(0);
}


/*
	row:����ĵڼ���
	ID:�û�ID
	port���û��˿�
	status���û�����״̬
	IP���û�IP
	QQ:�û�QQ
*/
void CFindDlg::ListAddRow(int row,int ID,int port,int status,CString IP,CString QQ)//ΪList�ؼ������
{
	CString str;


	str.Format("%04d",ID);
	int nIndex = m_find_list.InsertItem(row,str);

	str.Format("%04d",port);
	m_find_list.SetItemText(nIndex,1,str);

	switch (status){
	case 0:
		str = "ʹ����";
		break;
	case 1:
		str = "�ѹر�";
		break;
	case 2:
		str = "δ����";
		break;
	case 3:
		str = "������";
		break;
	default:
		str = "δ֪";
			break;
	}
	m_find_list.SetItemText(nIndex,2,str);

	m_find_list.SetItemText(nIndex,3,IP);
	m_find_list.SetItemText(nIndex,4,QQ);
}

void CFindDlg::ListAddLine(int line)//ΪList�ؼ������
{
	CRect rect;
	m_find_list.GetClientRect(&rect);//��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	m_find_list.SetExtendedStyle(m_find_list.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����

	m_find_list.InsertColumn(0,_T("ID"),LVCFMT_CENTER,rect.Width()/line);
	m_find_list.InsertColumn(1,_T("�˿�"),LVCFMT_CENTER,rect.Width()/line);//Ϊ�б���ͼ�ؼ������
	m_find_list.InsertColumn(2,_T("����״̬"),LVCFMT_CENTER,rect.Width()/line);
	m_find_list.InsertColumn(3,_T("IP"),LVCFMT_CENTER,rect.Width()/line);
	m_find_list.InsertColumn(4,_T("QQ"),LVCFMT_CENTER,rect.Width()/line);
}

void CFindDlg::OnBnClickedFindButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFile file;
	CUserInfo userinfo;
	CString FindString;
	int num,ret,i,port;
	LONGLONG seek[USERNUMMAX];
	BYTE userdata[USERINFOLEN];

	GetDlgItemText(IDC_FIND_EDIT,FindString);
	if((FindString == "")&&(m_combo.GetCurSel() !=1)
		&&(m_combo.GetCurSel() !=4)&&(m_combo.GetCurSel() !=5)&&(m_combo.GetCurSel() !=6))
	{
		MessageBox( TEXT(NInFindInfo) , TEXT(TiShi) ,MB_OK);
		return;
	}
	if(UserStatus == 0)//δ��¼
	{
		MessageBox( TEXT("δ��¼�˻������ȵ�¼�˺ţ�") , TEXT(TiShi) ,MB_OK);
		return;
	}

	m_find_list.DeleteAllItems();//ÿ�β�ѯ�����
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	switch (m_combo.GetCurSel())
	{
		case 0://�˿�
			port = _ttoi(FindString);
			if((port > 9999)||(port <= 0))
			{
				MessageBox( TEXT("������Ϸ��Ķ˿ڣ�") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			ret = CEditUserDlg::FindUserInfo(file,2,port,"",seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("δ��ѯ���ö˿ڣ�") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("��ѯ�û��˿ڴ���") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//ѭ���Ѳ�ѯ�����ʾ
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//��������
				ListAddRow(i,userinfo.userid,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
			}
			while(i != num);
			break;

		//�û�����״̬
		case 1://δ����
		case 4://������
		case 5://�ѹر�
		case 6://ʹ����
			int IntInfo;
			if(m_combo.GetCurSel() == 1) IntInfo = 2;
			else if(m_combo.GetCurSel() == 4) IntInfo = 3;
			else if(m_combo.GetCurSel() == 5) IntInfo = 1;
			else if(m_combo.GetCurSel() == 6) IntInfo = 0;

			ret = CEditUserDlg::FindUserInfo(file,4,IntInfo,FindString,seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("δ��ѯ��δ�����û���") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("��ѯδ�����û�����") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//ѭ���Ѳ�ѯ�����ʾ
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//��������
				ListAddRow(i,userinfo.userid,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
			}
			while(i != num);
			break;

		case 2://IP
			ret = CAddIPDlg::GetIPSeek(FindString,&port,file);//��ȡip��ƫ��port//���ֽ�����
			if(ret != 0)
			{
				MessageBox( TEXT("������Ϸ���IP��\r\n���磺255.255.255.1") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			ret = CEditUserDlg::FindUserInfo(file,5,port,"",seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("δ��ѯ���÷�������") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("��ѯ�û�����������") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//ѭ���Ѳ�ѯ�����ʾ
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//��������
				ListAddRow(i,userinfo.userid,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
			}
			while(i != num);
			break;

		case 3://QQ
			ret = CEditUserDlg::FindUserInfo(file,3,0,FindString,seek,&num);
			if(ret == 100)
			{
				MessageBox( TEXT("δ��ѯ����QQ�û���") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			else if(ret != 0)
			{
				MessageBox( TEXT("��ѯ�û�QQ����") , TEXT(TiShi) ,MB_OK);
				file.Close();
				return;
			}
			i = 0;//ѭ���Ѳ�ѯ�����ʾ
			do
			{
				file.Seek(seek[i],CFile::begin);
				file.Read(userdata,USERINFOLEN);
				i++;
				CEditUserDlg::UserDataExplan(userdata,userinfo);//��������
				ListAddRow(i,userinfo.userid,userinfo.port,userinfo.userstatus,CAddIPDlg::GetCommonIPForSel(file,userinfo.userip),userinfo.qqNum);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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


void CFindDlg::OnNMDblclkFindEditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)//��ѡ��
	{
		CString idnum = m_find_list.GetItemText(pNMListView->iItem, 0);//��ȡѡ���ID

		CEditUserDlg edituserdlg;
		edituserdlg.Buttonstatus = 4;//��ʾ��ϸ��Ϣ
		edituserdlg.IDNUM = _ttoi(idnum);//ת��������
		edituserdlg.DoModal();
	}
	*pResult = 0;
}


void CFindDlg::OnCbnSelchangeFindCombo()//�ؼ�comboѡ��֮���¼�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if((m_combo.GetCurSel() == 1)||//����״̬
		(m_combo.GetCurSel() == 4)||
		(m_combo.GetCurSel() == 5)||
		(m_combo.GetCurSel() == 6))
		
	{
		GetDlgItem(IDC_FIND_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_FIND_EDIT)->EnableWindow(TRUE);
	}
}


void CFindDlg::OnNMRClickFindEditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)//��ѡ��
	{
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos));

		CMenu menu;
		menu.LoadMenu(IDR_MENU);
		CMenu* popup = menu.GetSubMenu(0);

		popup->EnableMenuItem(ID_1,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //�������½��˵���ʹ��
		popup->EnableMenuItem(ID_2,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //������༭�˵���ʹ��
		popup->EnableMenuItem(ID_3,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //������ɾ���˵���ʹ��
		popup->EnableMenuItem(ID_4,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //������˵���ʹ��
		popup->EnableMenuItem(ID_5,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //���������IP�˵���ʹ��
		popup->EnableMenuItem(ID_6,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //������ɾ��IP�˵���ʹ��
		popup->EnableMenuItem(ID_7,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);  //���������IP�˵���ʹ��
		popup->EnableMenuItem(ID_8,MF_BYCOMMAND|MF_ENABLED);  //�������Ѳ˵���ʹ��

		CString idnum = m_find_list.GetItemText(pNMListView->iItem, 0);//��ȡѡ���ID
		IDNUM = _ttoi(idnum);//ת��������

		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	*pResult = 0;
}


void CFindDlg::On8()//����
{
	int ret;
	// TODO: �ڴ���������������
	CAddAmountDlg addamountdlg;
	addamountdlg.IDNUM = IDNUM;
	ret = addamountdlg.DoModal();
	if(ret == IDOK)
		MessageBox( TEXT("���ѳɹ���") , TEXT(TiShi) ,MB_OK);
}
