#include "StdAfx.h"
#include "UserInfo.h"

CUserInfo::CUserInfo()
{
}

CUserInfo::CUserInfo(int userid1,int port1,int userstatus1,CString qqNum1,CString userip1,
	CString password1,int amount1,CString lasttime1,CString startime1,CString endtime1)
{
	userid = userid1;//ΨһID
	port =port1;//�˿�
	userstatus = userstatus1;//�û�ʹ��״̬
	userstatus = userstatus1;//QQ����
	userip = userip1;//IP��ַ
	password = password1;//����

	//�ɷ���ϸ
	amount = amount1;//�ɷѽ��
	lasttime = lasttime1;//���ɷ�ʱ��
	startime = startime1;//����ɷ�ʱ��
	endtime = endtime1;//����ʱ��
}


CUserInfo::~CUserInfo(void)
{
}