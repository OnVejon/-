#pragma once
void * FuncSCallProxy1(char *pFuncName,int len,...);
void FuncSCallProxy2(char *pFuncName, long size,char *pParam,char *pRet);
char * FuncSCallProxy3(char *pName,char * pParam);