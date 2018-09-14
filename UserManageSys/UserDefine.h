#pragma once
#define uchar unsigned char
#define uint unsigned int

#define IP1 _T("45.63.111.81")
#define IP2 _T("63.210.148.252")

#define USERNUMMAX 100//允许存储最多的用户个数
#define IPNUMMAX 20//允许添加服务器数量的最大值

//各个数据的偏移地址
#define USERINFOHEADLEN 116//文件头预留116字节
#define USERINFOHEADADD 0//首地址

#define COMMONDATALEN 4*IPNUMMAX//共用数据存储区
#define COMMONDATAADD USERINFOHEADLEN

#define USERDATAADD (USERINFOHEADLEN+COMMONDATAADD)//用户数据起始地址



//用户数据偏移
#define STATUSADD 0
#define STATUSLEN sizeof(BYTE)//存储状态 0代表删除，1代表有效//1byte

#define USERIDADD (STATUSADD+STATUSLEN)
#define USERIDLEN sizeof(int)//唯一ID//4byte

#define PORTADD (USERIDADD+USERIDLEN)
#define PORTLEN sizeof(int)//端口//4byte

#define USERSTATUSADD (PORTADD+PORTLEN)
#define USERSTATUSLEN sizeof(int)//用户使用状态//4byte

#define QQNUMADD (USERSTATUSADD+USERSTATUSLEN)
#define QQNUMLEN (15*sizeof(BYTE))//QQ号码//15byte

#define USERIPADD (QQNUMADD+QQNUMLEN)//IP地址//15byte
#define USERIPLEN (15*sizeof(BYTE))

#define PASWORDADD (USERIPADD+USERIPLEN)//密码//15byte
#define PASWORDLEN (15*sizeof(BYTE))

#define AMOUNTADD (PASWORDADD+PASWORDLEN)//缴费金额//4byte
#define AMOUNTLEN sizeof(int)

#define LASTTIMEADD (AMOUNTADD+AMOUNTLEN)//最后缴费时间//15byte
#define LASTTIMELEN (15*sizeof(BYTE))

#define STARTIMEADD (LASTTIMEADD+LASTTIMELEN)//最初缴费时间//15byte
#define STARTIMELEN (15*sizeof(BYTE))

#define ENDTIMEADD (STARTIMEADD+STARTIMELEN)//到期时间//15byte
#define ENDTIMELEN (15*sizeof(BYTE))

#define FILLADD (ENDTIMEADD+ENDTIMELEN)//填充数据//14byte//保证数据为16的倍数方便加密
#define FILLLEN (14*sizeof(BYTE))

#define USERINFOLEN (FILLADD+FILLLEN)//一个用户数据的总的长度


#define TiShi _T("提示")
#define NFUserInfo _T("未查找到相关用户信息")
#define NInFindInfo _T("未输入查找的信息")
#define ClickCancel _T("点击取消")
#define ClickOk _T("点击确认")
#define NFDataFile _T("未查找到数据文件")
#define NIUserInfo _T("未输入账户密码")
#define UserNLogin _T("账户未注册，是否注册")
#define LoginNSame _T("两次输入密码不一致")
#define LoginSuccess _T("注册成功")
#define LoginErr _T("注册失败")
#define UserLenErr _T("账号长度需大于4，密码需大于6")
#define UserExtern _T("账户已存在请更换注册信息")
#define ReadFileErr _T("读取文件失败")
#define PasswordErr _T("密码输入错误！")
#define WecomeUser _T("欢迎使用用户管理系统！")
#define ExitUser _T("确认退出账户？")
#define NoNetWork _T("无网络，请检查网络")
#define UserLock _T("账户已被锁定，请在24小时后联网重试")

#define ErrCord2001 _T("2001")//错误码2001,注册文件生成失败
#define ErrCord2002 _T("2002")//错误码2002,读取文件长度错误
#define ErrCord2003 _T("2003")//错误码2003,读取文件校验值错误
#define ErrCord2004 _T("2004")//错误码2004,向文件添加IP出错