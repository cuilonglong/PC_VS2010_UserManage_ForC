// AddIPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "AddIPDlg.h"
#include "afxdialogex.h"
#include "Debug.h"
#include "UserDefine.h"
#include "UserManageSysDlg.h"
#include "EditUserDlg.h"


extern CString LogInUserName;
// CAddIPDlg �Ի���

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
	DDX_Control(pDX, IDC_ADDIP1_IPADDRESS, m_addip_address1);
}


BEGIN_MESSAGE_MAP(CAddIPDlg, CDialogEx)
	ON_BN_CLICKED(ID_SAVEIP_BUTTON, &CAddIPDlg::OnBnClickedSaveipButton)
END_MESSAGE_MAP()


// CAddIPDlg ��Ϣ�������


BOOL CAddIPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_addip_address.SetAddress(255,255,255,1);

	if(status == 0)//���IP
	{
		GetDlgItem(IDC_EDITIP_STATIC)->SetWindowText("������Ҫ���IP��");
	}
	else if(status == 1)
	{
		GetDlgItem(IDC_EDITIP_STATIC)->SetWindowText("������Ҫɾ��IP��");
	}
	else if(status == 2)
	{
		GetDlgItem(IDC_EDITIP_STATIC)->SetWindowText("������Ҫ���ĵ�IP��");
		GetDlgItem(IDC_EDITIP1_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ADDIP1_IPADDRESS)->ShowWindow(SW_SHOW);
		m_addip_address1.SetAddress(255,255,255,2);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAddIPDlg::OnBnClickedSaveipButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;
	BYTE IP[4];
	CFile file;
	CString readerrlog;

	m_addip_address.GetAddress(IP[0],IP[1],IP[2],IP[3]);
	//uart_printf("%d.%d.%d.%d\r\n",IP[0],IP[1],IP[2],IP[3]);
	
	//MessageBox( _T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix) , TEXT(TiShi) ,MB_OK);
	if(file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite))
	{
		if(status == 0)//���IP
		{
			ret = AddCommonFile(file,IP);
			if(ret < 0)
			{
				readerrlog.Format("%s [%s]",ReadFileErr, ErrCord2004); 
				MessageBox( _T(readerrlog) , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 1)
			{
				MessageBox( _T("���IP������󴢴�ֵ") , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 2)
			{
				MessageBox( _T("IP��ַ�Ѵ��ڣ������ظ����") , TEXT(TiShi) ,MB_OK);
			}
		}
		else if(status == 1)//ɾ��IP
		{
			ret = DelCommonFile(file,IP);
			if(ret < 0)
			{
				readerrlog.Format("%s [%s]",ReadFileErr, ErrCord2004); 
				MessageBox( _T(readerrlog) , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 1)
			{
				MessageBox( _T("û�д洢IP����") , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 2)
			{
				MessageBox( _T("δ����Ҫɾ����IP����") , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 3)
			{
				MessageBox( _T("���˻�ʹ�ø÷�����IP��������ɾ������") , TEXT(TiShi) ,MB_OK);
			}
		}
		else if(status == 2)//����IP
		{
			BYTE IP1[4];
			m_addip_address1.GetAddress(IP1[0],IP1[1],IP1[2],IP1[3]);
			ret = EditCommonFile(file,IP,IP1);
			if(ret == 1)
			{
				MessageBox( _T("δ��ѯ����Ҫ���ĵ�IP") , TEXT(TiShi) ,MB_OK);
			}
			else if(ret == 2)
			{
				MessageBox( _T("Ŀ��IP�Ѵ���,���޸�Ϊ������IP") , TEXT(TiShi) ,MB_OK);
			}
		}

		file.Close();
		if(ret == 0)
		{
			MessageBox( _T("�����ɹ�������") , TEXT(TiShi) ,MB_OK);
			goto Scucess;
		}		
	}
	else
	{
		MessageBox( _T(ReadFileErr) , TEXT(TiShi) ,MB_OK);
	}
	
	return;
Scucess:
	CDialogEx::OnOK();
}


//����IP��sel��ȡIP
//sel�������COMMONDATAADD��ַ
CString CAddIPDlg::GetCommonIPForSel(CFile &file,int sel)
{
	//int num;
	BYTE IP[COMMONDATALEN];
	CString str;

	//ReadCommonFile(file,&num,IP);
	file.Seek(COMMONDATAADD,CFile::begin);
	file.Read(IP,COMMONDATALEN);

	str.Format("%d.%d.%d.%d",IP[sel],IP[sel+1],IP[sel+2],IP[sel+3]);
	return str;
}

/*
�Թ����洢��IP���ݶ�ȡ����Ӻ�ɾ��
���ĸ�������ʱȫ������Ϊ��̬����
*/
int CAddIPDlg::ReadCommonFile(CFile &file,int *num,BYTE *IP)//�����ָ�볤��Ҫ������ֹ���
{
	int readlen;
	BYTE ReadIP[COMMONDATALEN];

	*num = 0;
	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	
	if((readlen % 4) != 0)//�ļ��쳣
	{
		return -1;
	}
	if(readlen == 0)//δ�洢
	{
		return 0;
	}

	do
	{
		readlen -= 4;
		if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//ȫ��Ϊ0��δ�洢����������
			continue;
		else //��IP
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
	int contrl = readlen;//����ѭ��
	if(readlen == 0)//��һ�����
	{
		file.Write(IP,4);
		return 0;
	}
	else if((readlen < 0)||(readlen > COMMONDATALEN))
	{
		return -2;
	}
	else
	{
		readlen = 0;
		do
		{
			if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//ȫ��Ϊ0�����˳���δ�洢��
				break;
			else if((ReadIP[readlen] == IP[0])&&(ReadIP[readlen + 1] == IP[1])&&
				(ReadIP[readlen + 2] == IP[2])&&(ReadIP[readlen + 3] == IP[3]))//���ظ�
			{
				return 2;
			}
			readlen += 4;
		}
		while(contrl != readlen);

		if(COMMONDATALEN == readlen)//�洢����
		{
			return 1;
		}

		file.Seek(COMMONDATAADD + readlen,CFile::begin);
		file.Write(IP,4);
		return 0;
	}

	return 0;
}

int CAddIPDlg::DelCommonFile(CFile &file,BYTE *IP)// 0�ɹ���1ʧ��
{
	int readlen;
	CString password;
	CUserInfo userinfo;
	BYTE ReadIP[COMMONDATALEN];
	int errnum,time[4],usernum,ret,num,ipsel = 0;

	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	
	if((readlen % 4) != 0)//�ļ��쳣
	{
		ret = -1;
		goto Err;
	}
	if(readlen == 0)
	{
		ret = 1;
		goto Err;
	}
	
	do
	{
		readlen -= 4;
		if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//ȫ��Ϊ0��δ�洢����������
			continue;
		else if((ReadIP[readlen] == IP[0])&&(ReadIP[readlen + 1] == IP[1])&&
			(ReadIP[readlen + 2] == IP[2])&&(ReadIP[readlen + 3] == IP[3]))//��ȫ��ͬ
		{
			goto Find;
		}
	}
	while(0 != readlen);
	if(readlen == 0)//�����ڸ�IP
	{
		ret = 2;
		goto Err;
	}

Find:
	//����һ��IPδʹ�ò�����ɾ��
	ret = CUserManageSysDlg::ExplainFileHead(file,LogInUserName,password,errnum,time,usernum);//��ȡ�Ѿ��洢���û�����
	if(ret != 0){
		::MessageBox( NULL,_T("ɾ�����������ݻ�ȡ�û�����ʧ�ܣ�") , TEXT(TiShi) ,MB_OK);
		ret = -2;
		goto Err;
	}
	if(usernum == 0)//���û�����
	{
		goto Scucess;
	}
	ret = ReadCommonFile(file,&num,ReadIP);//��ȡ���е�IP����
	if(ret != 0)
	{
		goto Err;
	}
	do//ѭ������
	{
		if(!memcmp(ReadIP+ipsel*4,IP,4))//��ͬΪ0
		{
			break;
		}
		ipsel++;
	}
	while(ipsel < num);

	int index = 0;
	num = 0;
	do
	{
		ret = CEditUserDlg::ReadUserInfo(file,userinfo,num++);
		if(ret != 0)
			continue;
		if(userinfo.userip == ipsel)
		{
			ret =3;
			goto Err;
		}
		index++;
		if(index == usernum)//�������
			goto Scucess;
	}
	while(index <= USERNUMMAX);

Err:
	return ret;

Scucess:
	memset(ReadIP + readlen,0,4);
	file.Seek(COMMONDATAADD + readlen,CFile::begin);
	file.Write(ReadIP + readlen,4);
	return 0;
}

int CAddIPDlg::EditCommonFile(CFile &file,BYTE *IP,BYTE *IP1)//�༭IP
{
	int readlen,ret,index = 0;
	BYTE ReadIP[COMMONDATALEN];

	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	if((readlen % 4) != 0)//�ļ��쳣
	{
		ret = -1;
		goto Err;
	}
	if(readlen == 0)
	{
		ret = 1;//δ�洢IP
		goto Err;
	}

	do//������ѯ�Ƿ�����滻��IP1
	{
		if(!memcmp(ReadIP+index,IP1,4))//��ͬΪ0
		{
			ret = 2;
			goto Err;
		}
		index += 4;
	}
	while(index != readlen);

	index = 0;
	do//������ѯ�Ƿ���ڴ��滻��IP
	{
		if(!memcmp(ReadIP+index,IP,4))//��ͬΪ0
		{
			file.Seek(COMMONDATAADD + index,CFile::begin);
			file.Write(IP1,4);
			goto Scucess;
		}
		index += 4;
	}
	while(index != readlen);
	if(index == readlen)
	{
		ret = 1;//����δ���ҵ���ӦIP
		goto Err;
	}
Err:
	return ret;
Scucess:
	return 0;
}

/*
��ȡIP��ַ�ľ���ƫ�Ƶ�ַ
seek ������COMMONDATAADD�ĳ���
IP��ʽ����Ϊ"255.255.255.1"��ʽ
*/
int CAddIPDlg::GetIPSeek(CString IP,int *seek)
{
	CString str;
	CFile file;
	int readlen,ret,index = 0;
	BYTE ReadIP[COMMONDATALEN];
	
	file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	if((readlen % 4) != 0)//�ļ��쳣
	{
		ret = -1;
		goto Err;
	}
	if(readlen == 0)
	{
		ret = 1;//δ�洢IP
		goto Err;
	}

	do//������ѯ�Ƿ�����滻��IP1
	{
		str.Format("%d.%d.%d.%d",ReadIP[index],ReadIP[index+1],ReadIP[index+2],ReadIP[index+3]);
		if(str == IP)//���ҵ�
		{
			*seek = index;
			goto Scucess;
		}
		index += 4;
	}
	while(index != readlen);
	if(index == readlen)
		ret =1;//δ�鵽

Err:
	file.Close();
	return ret;
Scucess:
	file.Close();
	return 0;
}

int CAddIPDlg::GetIPSeek(CString IP,int *seek,CFile &file)
{
	CString str;
	//CFile file;
	int readlen,ret,index = 0;
	BYTE ReadIP[COMMONDATALEN];
	
	//file.Open(_T(CUserManageSysDlg::GetFilePath()+ LogInUserName + FileSuffix/*�ļ�β׺*/), CFile::modeReadWrite);
	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	if((readlen % 4) != 0)//�ļ��쳣
	{
		ret = -1;
		goto Err;
	}
	if(readlen == 0)
	{
		ret = 1;//δ�洢IP
		goto Err;
	}

	do//������ѯ�Ƿ�����滻��IP1
	{
		str.Format("%d.%d.%d.%d",ReadIP[index],ReadIP[index+1],ReadIP[index+2],ReadIP[index+3]);
		if(str == IP)//���ҵ�
		{
			*seek = index;
			goto Scucess;
		}
		index += 4;
	}
	while(index != readlen);
	if(index == readlen)
		ret =1;//δ�鵽

Err:
	//file.Close();
	return ret;
Scucess:
	//file.Close();
	return 0;
}