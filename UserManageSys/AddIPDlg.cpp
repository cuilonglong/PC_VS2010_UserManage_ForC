// AddIPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserManageSys.h"
#include "AddIPDlg.h"
#include "afxdialogex.h"
#include "Debug.h"
#include "UserDefine.h"
#include "UserManageSysDlg.h"


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
		}

		if(ret == 0)
		{
			MessageBox( _T("�����ɹ�������") , TEXT(TiShi) ,MB_OK);
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
	if(readlen == 0)//��һ�����
	{
		file.Write(IP,4);
		return 0;
	}
	else if((readlen < COMMONDATALEN)&&(readlen > 0))//�ڶ���֮�����
	{
		if((readlen % 4) != 0)//�ļ��쳣
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
			if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//ȫ��Ϊ0�����˳���δ�洢��
				break;
			else if((ReadIP[readlen] == IP[0])&&(ReadIP[readlen + 1] == IP[1])&&
				(ReadIP[readlen + 2] == IP[2])&&(ReadIP[readlen + 3] == IP[3]))//���ظ�
			{
				return 2;
			}
			readlen += 4;
		}
		while(COMMONDATALEN != readlen);

		if(COMMONDATALEN == readlen)//�洢����
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
int CAddIPDlg::DelCommonFile(CFile &file,BYTE *IP)// 0�ɹ���1ʧ��
{
	int readlen;
	BYTE ReadIP[COMMONDATALEN];

	file.Seek(COMMONDATAADD,CFile::begin);
	readlen = file.Read(ReadIP,COMMONDATALEN);
	
	if((readlen % 4) != 0)//�ļ��쳣
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
		if(!(ReadIP[readlen]|ReadIP[readlen+1]|ReadIP[readlen+2]|ReadIP[readlen+3]))//ȫ��Ϊ0��δ�洢����������
			continue;
		else if((ReadIP[readlen] == IP[0])&&(ReadIP[readlen + 1] == IP[1])&&
			(ReadIP[readlen + 2] == IP[2])&&(ReadIP[readlen + 3] == IP[3]))//��ȫ��ͬ
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