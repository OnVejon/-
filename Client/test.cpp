#include "Client.h"

#include <iostream>
int test1()
{
	char *pFuncNmae = "student.fun1";
	char *pName = "tom";
	long lName = strlen(pName);
	int age = 18;
	long lage = sizeof(age);
	void *pRet;
	pRet = FuncSCallProxy1(pFuncNmae, 2, 's', lName, pName, 'd', lage, age);
	printf((char *)pRet);
	delete[] pRet;
	return 0;
}
int test2()
{
	char *pFuncNmae = "student.fun1";
	char *pName = "tom";
	long lName = strlen(pName);
	int age = 18;
	long lage = sizeof(age);
	void *pRet;
	pRet = FuncSCallProxy1(pFuncNmae, 2, 's', lName, pName, 'd', lage, age);
	printf((char *)pRet);
	delete[] pRet;
	return 0;
}
int test3()
{
	char *pFuncNmae = "student.fun1";
	char *pName = "tom";
	long lName = strlen(pName);
	int age = 18;
	long lage = sizeof(age);
	void *pRet;
	pRet = FuncSCallProxy1(pFuncNmae, 2, 's', lName, pName, 'd', lage, age);
	printf((char *)pRet);
	delete[] pRet;
	return 0;
}

void main()
{
	test1();
}