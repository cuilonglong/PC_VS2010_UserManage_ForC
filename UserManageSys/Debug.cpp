
#include "StdAfx.h"
#include "atlbase.h"
#include "atlstr.h"
#include "Debug.h"

//��ӡ����
void uart_printf(const char* Output,...)
{
    char strBuffer[4096]={0};
	CString outputstring;

    va_list vlArgs;
    va_start(vlArgs,Output);
    _vsnprintf_s(strBuffer,sizeof(strBuffer)-1,Output,vlArgs);
    va_end(vlArgs);
    //OutputDebugString(CA2W(strBuffer));

	outputstring.Format("%s",strBuffer);
	OutputDebugString(outputstring);
}

//��ӡ�ַ���
void printf_string(CString string)
{
	uart_printf(string.GetBuffer());
	return;
}

void printf_format(char * Output,int len)
{
	int i;
	uart_printf("��ӡ�ַ�����Ϊ%d\r\n",len);
	for(i = 0;i<len;i++)
	{
		if(i % 16 == 0)
			uart_printf("\r\n");
		uart_printf("%02x ",(unsigned char)(Output[i]));
	}
	uart_printf("\r\n");
	return;
}