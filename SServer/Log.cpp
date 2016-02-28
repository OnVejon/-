#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "Log.h"


#define LogFile "G:\\httpservice.log"



void Log(char* str)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	
	fstream fp(LogFile, ios::app);

	fp <<sys.wYear<<"-"<<sys.wMonth<<"-"<<sys.wDay<<" "<<sys.wHour<<":"<<sys.wMinute<<":"<<sys.wSecond<<"\t"<< str << endl;

	fp.close();
	
}
void Log(string str)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	fstream fp(LogFile, ios::app);

	fp << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond <<  "\r" << str.c_str() << endl;

	fp.close();

}