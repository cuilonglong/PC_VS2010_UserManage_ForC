#pragma once
class GetNetTime
{
public:
	static int getnettime(SYSTEMTIME &st);
	static void ConvertTime(unsigned long ulTime,SYSTEMTIME &st);
};

