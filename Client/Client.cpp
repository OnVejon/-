#include "../include/Client.h"
#include "../SServer/socket.h"
#include <WS2tcpip.h>

#define clink 1
#define cpass 2
#define cfun  3

typedef struct {
	char ch;
	long size;
	void *str;
}StructMode1;


int ConnectFunc(char **out, CallHead head, char * Param);
int InputPassword(SOCKET hSocket)
{
	user us;
	printf("连接成功，请输入账号：\n");
	scanf("%s ", us.name);
	printf("连接成功，请输入密码：\n");
	scanf("%s", us.pass);
	return send(hSocket, (char *)&us, MAX_BUF_SIZE, 0);
}

void * FuncSCallProxy1(char *pFuncName, int len, ...)
{
	char *out=NULL;
	CallHead head = {1,0,""};
	
	char * Param;
	strcpy_s(head.pFuncName, pFuncName);
	StructMode1 * smode1 = new StructMode1[len];

	va_list arg_ptr;
	va_start(arg_ptr, len);
	
	for (int i = 0; i < len; i++)
	{
		smode1[i].ch = va_arg(arg_ptr, char);
		smode1[i].size= va_arg(arg_ptr, char);
		smode1[i].str = va_arg(arg_ptr, void *);
		
	}
	va_end(arg_ptr);
	char *tmp1=NULL, *tmp2=NULL;
	char tmp[255];
	for (int i = 0; i < len; i++)
	{
		switch(smode1[i].ch)
		{
		case 's':
		case 'S':tmp2 = tmp1;
			if (tmp2) 
			{	head.buflen += smode1[i].size+1;  tmp1 = new char[head.buflen]; 
				sprintf(tmp1, "%s\n%s",tmp2, ((char *)smode1[i].str)); delete[] tmp2; tmp2 = NULL;
			}/*接在后面*/
			else {
				head.buflen += smode1[i].size;  tmp1 = new char[head.buflen];
				sprintf(tmp1, "%s", ((char *)smode1[i].str)); }/*第一个*/ break;
		case 'c':
		case 'C':tmp2 = tmp1;
			if (tmp2)
			{
				head.buflen += smode1[i].size + 1;  tmp1 = new char[head.buflen];
				sprintf(tmp1, "%s\n%c", tmp2, *((char *)smode1[i].str)); delete[] tmp2; tmp2 = NULL;
			}
			else {
				head.buflen += smode1[i].size;  tmp1 = new char[head.buflen];
				sprintf(tmp1, "%c", *((char *)smode1[i].str));
			} break;
		case 'f':
		case 'F': sprintf(tmp,"%f\n", *(double *)&smode1[i].str); 
			tmp2 = tmp1;
			if (tmp2)
			{
				head.buflen += strlen(tmp) + 1;  tmp1 = new char[head.buflen];
				sprintf(tmp1, "%s\n%s", tmp2,tmp); delete[] tmp2; tmp2 = NULL;
			}
			else {
				head.buflen += strlen(tmp);  tmp1 = new char[head.buflen];
				sprintf(tmp1, "%s", tmp);
			} break;
		case 'd':
		case 'D': sprintf(tmp, "%f\n", *(int *)&smode1[i].str);
			tmp2 = tmp1;
			if (tmp2)
			{
				head.buflen += strlen(tmp) + 1;  tmp1 = new char[head.buflen];
				sprintf(tmp1, "%s\n%s", tmp2, tmp);
				
			}
			else {
				head.buflen += strlen(tmp);  tmp1 = new char[head.buflen];
				sprintf(tmp1, "%s", tmp);
			} break;
		default:;
		}
		
		
	}
	Param = tmp1;
	ConnectFunc(&out,head, Param);

	delete[] smode1;
	return out;
}
void FuncSCallProxy2(char *pFuncName, long size, char *pParam, void *pRet)
{
	char *Ret = NULL;
	CallHead head;
	head.callmode = 2;
	head.buflen = size;
	strcpy_s(head.pFuncName, pFuncName);
	ConnectFunc(&Ret, head, pParam);
	strcpy((char *)pRet, Ret);
	
}
char * FuncSCallProxy3(char *pName, char * pParam)
{
	char *Ret = NULL;
	CallHead head;
	head.callmode = 3;
	head.buflen = strlen(pParam);
	strcpy_s(head.pFuncName, pName);
	ConnectFunc(&Ret, head, pParam);
	return Ret;
}


int ConnectFunc(char **out, CallHead head, char * Param)
{
	SOCKET ContSock;
	SOCKADDR_IN ContAddr;

	int mode = clink;
	if (WSAInit() == FALSE)
	{
		puts("WSA ERROR");
		goto __Error_End;
	}
	if (SocketInit(&ContSock) == FALSE)
	{
		puts("Listen Socket Init ERROR");
		goto __Error_End;
	}

	SockAddrInit(&ContAddr,FALSE);

	if (connect(ContSock, (SOCKADDR *)&ContAddr, sizeof(ContAddr)))
	{
		puts("connect error!");
		return 0;
	}
SENDHELLO:
	SENDbuf(ContSock, CLIENTHELLO);
	while (TRUE)
	{
		if (SOCKET_Select( ContSock))
		{
			char szBuf[MAX_BUF_SIZE] = { 0 };
			int iRet = recv(ContSock, szBuf, MAX_BUF_SIZE, 0);
			if (iRet > 0) {
				//right;
				if (!strcmp(szBuf, SERVEROK))
				{
					if (mode == clink)
					{
						InputPassword(ContSock);
						mode = cpass;
					}
					if (mode == cpass)
					{
						send(ContSock, (char *)&head, sizeof(CallHead), 0);
						Sleep(50);
						send(ContSock, Param, head.buflen, 0);

						mode = cfun;
					}
					if (mode == cfun)
					{
						CallHead rHead;
						while (!SOCKET_Select(ContSock))Sleep(500);
						recv(ContSock, (char *)&rHead, sizeof(CallHead), 0);
						char *pRet = new char[rHead.buflen];
						while (!SOCKET_Select(ContSock))Sleep(500);
						recv(ContSock, pRet, rHead.buflen, 0);
						*out = pRet;
						return rHead.buflen;
					}

				}
				else if (!strcmp(szBuf, ERROR0))
				{
					printf("错误应答：%s\n重新等待应答\n", szBuf);
					goto SENDHELLO;
				}
				else if(!strcmp(szBuf, ERROR1))
				{
					printf("错误应答：%s\n重新等待应答\n", szBuf);
					InputPassword(ContSock);
					
				}else 
				{
					printf("错误应答：%s\n重新等待应答\n", szBuf);
					
				}
			}
			else {
				//close socket;
				printf("连接已断开\n");
				break;
			}
		}
	}



__Error_End:
	closesocket(ContSock);

	return 0;
}