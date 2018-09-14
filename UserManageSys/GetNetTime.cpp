#include "StdAfx.h"
#include "GetNetTime.h"
#include "Debug.h"
#include <math.h>

int GetNetTime::getnettime(	SYSTEMTIME &st)//��ȡ��UTCʱ�䣬�ͱ���ʱ���8Сʱ
{
    TIME_ZONE_INFORMATION tzinfo;
    DWORD dwStandardDaylight;
    int nRet;
 
    /* Initialize Winsock */
    WORD wVersionRequested;
    WSADATA wsaData;
    int nErrCode;
	
	wVersionRequested = MAKEWORD(2, 2);
    nErrCode = WSAStartup(wVersionRequested, &wsaData);
    if (0 != nErrCode)
    {
        return -1;
    }
 
	/* Get server IP */
	struct hostent *host;
	char *pServerIP;
 
	host = gethostbyname("time.nist.gov");
	if (NULL == host)
	{
		return -1;
	}
 
	pServerIP = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
 
	/* Connect to time server, and get time */
	SOCKET sockfd;
 
	char cTimeBuf[40] = { 0 };
	unsigned long ulTime = 0;
	int nTry = 0;
 
	do 
	{
		if (5 == nTry++)
		{
			return -1;
		}
 
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == sockfd)
		{
			continue;
		}
 
		int TimeOut = 3000;//��ʱʱ��
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));
 
		sockaddr_in addr;
 
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(37);
		addr.sin_addr.s_addr = inet_addr(pServerIP);
 
		nRet = connect(sockfd, (sockaddr *)&addr, sizeof(addr));
		if (SOCKET_ERROR == nRet)
		{
			continue;
		}
 
		nRet = recv(sockfd, (char *)&ulTime, sizeof(ulTime), 0);
		if ((SOCKET_ERROR != nRet) && (0 != nRet))
		{
			break;
		}
 
		int nErr = WSAGetLastError();
		//TRACE(_T("[%d]%s"), nErr, ConvertErrcodeToString(nErr));
 
		closesocket(sockfd);
	} while (1);
	closesocket(sockfd);
 
	unsigned long ulTimehl = ntohl(ulTime);
	ConvertTime(ulTimehl,st);
	return 0;
}   

void GetNetTime::ConvertTime(unsigned long ulTime, SYSTEMTIME &st)
{
    // Windows�ļ�ʱ����һ��64λ��ֵ�����Ǵ�1601��1��1������12:00�����ڵ�ʱ������
    // ��λ��1/10,000,000�룬��1000���֮1��(100-nanosecond)
    FILETIME ft;
    //SYSTEMTIME st;
 
    // ���Ƚ���׼ʱ�䣨1900��1��1��0��0��0��0���룩ת��ΪWindows�ļ�ʱ��
    st.wYear = 1900;
    st.wMonth = 1;
    st.wDay = 1;
    st.wHour = 0;
    st.wMinute = 0;
    st.wSecond = 0;
    st.wMilliseconds = 0;
 
    SystemTimeToFileTime(&st, &ft);
 
    // Ȼ��Time Protocolʹ�õĻ�׼ʱ�������ȥ��ʱ�䣨ulTime��
    LONGLONG *pLLong = (LONGLONG *)&ft;
 
    /* ע�⣺
       �ļ�ʱ�䵥λ��1/1000 0000��(��100ns)��
       ��Ҫ����ʱ��������ϻ�ȡ������Ϊ��λ��ulTime��һ��ת��
    */
    *pLLong += (LONGLONG) 10000000 * ulTime;
 
    // �ٽ�ʱ��ת������������ϵͳʱ��
    FileTimeToSystemTime(&ft, &st);
 
   // uart_printf(_T("%04d%02d%02d %02d:%02d:%02d\n"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
 
    return;
}


