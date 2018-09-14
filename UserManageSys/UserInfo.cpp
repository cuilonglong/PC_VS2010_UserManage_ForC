#include "StdAfx.h"
#include "UserInfo.h"

CUserInfo::CUserInfo()
{
}

CUserInfo::CUserInfo(int userid1,int port1,int userstatus1,CString qqNum1,CString userip1,
	CString password1,int amount1,CString lasttime1,CString startime1,CString endtime1)
{
	userid = userid1;//唯一ID
	port =port1;//端口
	userstatus = userstatus1;//用户使用状态
	userstatus = userstatus1;//QQ号码
	userip = userip1;//IP地址
	password = password1;//密码

	//缴费详细
	amount = amount1;//缴费金额
	lasttime = lasttime1;//最后缴费时间
	startime = startime1;//最初缴费时间
	endtime = endtime1;//到期时间
}


CUserInfo::~CUserInfo(void)
{
}