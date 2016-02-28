// CreateLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "funlib.h"
using namespace std;

//int get_file_size(const char *filename);
//
//int FBN2FN(char Fun_Name[],char FunBodyName[])
//{
//
//	
//	if (strcpy(Fun_Name, FunBodyName) == NULL)
//	{
//		printf("error strcpy");
//		return 0;
//	}
//	int i = 0;
//	while (Fun_Name[i] == '\0')
//	{
//		if (Fun_Name[i] == '\\'|| Fun_Name[i] == '/')
//		{
//			Fun_Name[i] = '.';
//		}
//		i++;
//	}
//	while (Fun_Name[i] == '.')
//	{
//		Fun_Name[i] = '\0';
//		i--;
//	}
//	Fun_Name[i-1] = '\0';
//	return 1;
//}

void help()
{
	printf("controlLib 的命令行格式。\n"
		"\"libname +  funname funfilename\"函数体加入库\n"
		"\"libname -  funname\"函数体从库中移出\n"
		"\"libname -s\" 显示所有的lib中的函数体信息。\n"
		"\"libname -r funname newfunname \" 更改函数名\n"
		);
}

/*libname + dll + dll*/
int main(int argc, char **argv)
{
	char *LibName=NULL;
	

	int addi = 0, subi = 0;
	int i = 1;

	if (argc == 1|| !strcmp(argv[i], "-h"))
	{
		help();
		return 0;
	}
	for (i = 1; i < argc; ++i)
	{
		if (i == 1)
		{
			LibName = argv[i];
		}
		else if (!strcmp(argv[i], "+"))
		{
			//bool addFun(char * libName, char * funName,char * funFileName)
			addFun(LibName, argv[i+1], argv[i+2]);
			i += 2;
		}
		else if (!strcmp(argv[i], "-"))
		{
			delFun(LibName, argv[++i]);
		}else if (!strcmp(argv[i], "-s"))
		{
			showFun(LibName);
		}else if (!strcmp(argv[i], "-r"))
		{
			reFunName(LibName, argv[i+1],argv[i+2]);
			i += 2;
		}


	}

	return 0;
}










	/*char *LibName;
	char *addList[20], *subList[20];

	int addi = 0, subi = 0;
	int i = 1;

	if (argc == 1)
	{
		printf("Syntax: TLIB libname commands\n"
			"libname     library file pathname\n"
			"commands    sequence of operations to be performed(optional)\n"
			"listfile    file name for listing file(optional)\n"
			"A command is of the form : <symbol>modulename, where <symbol> is :\n"
			"+ add modulename to the library\n"
			"- remove modulename from the library\n"
			"Use @filepath to continue from file \"filepath\".\n"
			"Use '&' at end of a line to continue onto the next line.\n"
			);
		return 0;
	}

	for (i = 1; i < argc; ++i)
	{
		if (i == 1)
		{
			LibName = argv[i];
		}
		else if (!strcmp(argv[i], "+"))
		{
			addList[addi++] = argv[++i];
		}
		else if (!strcmp(argv[i], "-"))
		{
			subList[subi++] = argv[++i];
		}
		
	}
	char superchar[] = { 0x0a,0x1a,0x00 };
	FILE *fLib;
	fopen_s(&fLib,LibName, "r");
	if (fLib == NULL)
	{
		fopen_s(&fLib,LibName, "w");
		fprintf(fLib, "Function Server Libs v1.0%s",superchar);
		
		
	}
	fclose(fLib);
	


	char Funhead[]="Func Block";
	int Funmax;
	char FunName[256] = {0};
	int FunNamei = 0;
	FILE *fdll;
	addi = 0;
	while (addList[addi] != NULL)
	{
		Funmax = get_file_size(addList[addi])+10+4+255;

		

		fopen_s(&fLib, LibName, "a");
		fprintf(fLib, "%s%x", Funhead, Funmax);
		while (FunNamei+strlen(addList[addi]) < 256)
		{
			fputc('\0', fLib);
			FunNamei++;
		}
		addi++;*/
		/*fopen_s(&fdll, addList[addi], "r");
		if (fdll == NULL)
		{
			printf("文件%s不能打开", addList[addi]);
			return 0;
		}*/

//	}
	
	//fclose(fLib);
//}

//int get_file_size(const char *filename)
//{
//	unsigned long size;
//	FILE* fp;
//	fopen_s(&fp,filename, "rb");
//	if (fp == NULL) {
//		printf("ERROR: Open file %s failed.\n", filename);
//		return 0;
//	}
//	fseek(fp, 0L, SEEK_END);
//	size = ftell(fp);
//	fclose(fp);
//	return size;
//}