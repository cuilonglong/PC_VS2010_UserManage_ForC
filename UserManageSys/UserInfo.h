#pragma once
static CString FileSuffix = ".dat";//�ļ�β׺
//static int UserStatus = 0;//����ĵ�½״̬

typedef struct _DataHead//�����ļ�ͷ//���Ľ����������Ӳ�����������Ե�ַ
{
	int headlen;
	int AccountLen;
	CString Account;
	int PassworldLen;
	CString Passworld;
	int InputErrNum;//�������Ĵ���
	int UserNum;//�Ѵ洢���˻�����
	int Time[4];
	char HCV[16];
}DataHeard;

//typedef struct _UserFileInfo//�û���Ϣ
//{
//	int ID;//�û�ID
//	int seek;//���Ե�ַ
//	int filelen;//���û������ݳ���
//}UserFileInfo;

class CUserInfo
{
public:
	CUserInfo();
	CUserInfo(int userid1,int port1,int userstatus1,CString qqNum1,int userip1,
		CString passworld1,int amount1,CString lasttime1,CString startime1,CString endtime1);
	~CUserInfo(void);

	BYTE status;//�洢״̬ 0����ɾ����1������Ч//1byte
	int userid;//ΨһID//4byte
	int port;//�˿�//4byte
	int userstatus;//�û�ʹ��״̬//4byte

	CString qqNum;//QQ����//15byte
	int userip;//IP��ַ//4byte
	CString password;//����//15byte

	//�ɷ���ϸ
	int amount;//�ɷѽ��//4byte
	CString lasttime;//���ɷ�ʱ��//15byte
	CString startime;//����ɷ�ʱ��//15byte
	CString endtime;//����ʱ��//15byte
};

