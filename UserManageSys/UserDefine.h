#pragma once
#define uchar unsigned char
#define uint unsigned int

#define IP1 _T("45.63.111.81")
#define IP2 _T("63.210.148.252")

#define USERNUMMAX 100//����洢�����û�����
#define IPNUMMAX 20//������ӷ��������������ֵ

//�������ݵ�ƫ�Ƶ�ַ
#define USERINFOHEADLEN 116//�ļ�ͷԤ��116�ֽ�
#define USERINFOHEADADD 0//�׵�ַ

#define COMMONDATALEN 4*IPNUMMAX//�������ݴ洢��
#define COMMONDATAADD USERINFOHEADLEN

#define USERDATAADD (USERINFOHEADLEN+COMMONDATAADD)//�û�������ʼ��ַ



//�û�����ƫ��
#define STATUSADD 0
#define STATUSLEN sizeof(BYTE)//�洢״̬ 0����ɾ����1������Ч//1byte

#define USERIDADD (STATUSADD+STATUSLEN)
#define USERIDLEN sizeof(int)//ΨһID//4byte

#define PORTADD (USERIDADD+USERIDLEN)
#define PORTLEN sizeof(int)//�˿�//4byte

#define USERSTATUSADD (PORTADD+PORTLEN)
#define USERSTATUSLEN sizeof(int)//�û�ʹ��״̬//4byte

#define QQNUMADD (USERSTATUSADD+USERSTATUSLEN)
#define QQNUMLEN (15*sizeof(BYTE))//QQ����//15byte

#define USERIPADD (QQNUMADD+QQNUMLEN)//IP��ַ//15byte
#define USERIPLEN (15*sizeof(BYTE))

#define PASWORDADD (USERIPADD+USERIPLEN)//����//15byte
#define PASWORDLEN (15*sizeof(BYTE))

#define AMOUNTADD (PASWORDADD+PASWORDLEN)//�ɷѽ��//4byte
#define AMOUNTLEN sizeof(int)

#define LASTTIMEADD (AMOUNTADD+AMOUNTLEN)//���ɷ�ʱ��//15byte
#define LASTTIMELEN (15*sizeof(BYTE))

#define STARTIMEADD (LASTTIMEADD+LASTTIMELEN)//����ɷ�ʱ��//15byte
#define STARTIMELEN (15*sizeof(BYTE))

#define ENDTIMEADD (STARTIMEADD+STARTIMELEN)//����ʱ��//15byte
#define ENDTIMELEN (15*sizeof(BYTE))

#define FILLADD (ENDTIMEADD+ENDTIMELEN)//�������//14byte//��֤����Ϊ16�ı����������
#define FILLLEN (14*sizeof(BYTE))

#define USERINFOLEN (FILLADD+FILLLEN)//һ���û����ݵ��ܵĳ���


#define TiShi _T("��ʾ")
#define NFUserInfo _T("δ���ҵ�����û���Ϣ")
#define NInFindInfo _T("δ������ҵ���Ϣ")
#define ClickCancel _T("���ȡ��")
#define ClickOk _T("���ȷ��")
#define NFDataFile _T("δ���ҵ������ļ�")
#define NIUserInfo _T("δ�����˻�����")
#define UserNLogin _T("�˻�δע�ᣬ�Ƿ�ע��")
#define LoginNSame _T("�����������벻һ��")
#define LoginSuccess _T("ע��ɹ�")
#define LoginErr _T("ע��ʧ��")
#define UserLenErr _T("�˺ų��������4�����������6")
#define UserExtern _T("�˻��Ѵ��������ע����Ϣ")
#define ReadFileErr _T("��ȡ�ļ�ʧ��")
#define PasswordErr _T("�����������")
#define WecomeUser _T("��ӭʹ���û�����ϵͳ��")
#define ExitUser _T("ȷ���˳��˻���")
#define NoNetWork _T("�����磬��������")
#define UserLock _T("�˻��ѱ�����������24Сʱ����������")

#define ErrCord2001 _T("2001")//������2001,ע���ļ�����ʧ��
#define ErrCord2002 _T("2002")//������2002,��ȡ�ļ����ȴ���
#define ErrCord2003 _T("2003")//������2003,��ȡ�ļ�У��ֵ����
#define ErrCord2004 _T("2004")//������2004,���ļ����IP����