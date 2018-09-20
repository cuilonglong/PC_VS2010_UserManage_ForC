#pragma once
static CString FileSuffix = ".dat";//文件尾缀
//static int UserStatus = 0;//软件的登陆状态

typedef struct _DataHead//数据文件头//更改建议在最后添加不更改上面绝对地址
{
	int headlen;
	int AccountLen;
	CString Account;
	int PassworldLen;
	CString Passworld;
	int InputErrNum;//输入错误的次数
	int UserNum;//已存储的账户个数
	int Time[4];
	char HCV[16];
}DataHeard;

//typedef struct _UserFileInfo//用户信息
//{
//	int ID;//用户ID
//	int seek;//绝对地址
//	int filelen;//该用户的数据长度
//}UserFileInfo;

class CUserInfo
{
public:
	CUserInfo();
	CUserInfo(int userid1,int port1,int userstatus1,CString qqNum1,int userip1,
		CString passworld1,int amount1,CString lasttime1,CString startime1,CString endtime1);
	~CUserInfo(void);

	BYTE status;//存储状态 0代表删除，1代表有效//1byte
	int userid;//唯一ID//4byte
	int port;//端口//4byte
	int userstatus;//用户使用状态//4byte

	CString qqNum;//QQ号码//15byte
	int userip;//IP地址//4byte
	CString password;//密码//15byte

	//缴费详细
	int amount;//缴费金额//4byte
	CString lasttime;//最后缴费时间//15byte
	CString startime;//最初缴费时间//15byte
	CString endtime;//到期时间//15byte
};

