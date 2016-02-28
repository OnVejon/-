#pragma once
#include <string>
#define FUNNAMELEN 255
#define LIBHEADLEN 28
#define FBHEADLEN 10
#define FSIZELEN 4
typedef struct  {
	char FunFileName[FUNNAMELEN];
	char FunName[FUNNAMELEN];
	unsigned long Funsize;
	
}InFunBody;

typedef struct oFunBody {
	char PathName[FUNNAMELEN];
	char FunName[FUNNAMELEN];
	int begin;
	int end;
}OutFunBody;

bool addFun(char * libName, char * funName, char * funFileName);
bool reFunName(char *libName, char *funName, char * newFunName);
bool showFun(char *libName);
bool delFun(char *libName, char *funName);

