#pragma once
#include <winsock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#define INTADDR "127.0.0.1"
#define PORT 789

#define MAX_BUF_SIZE 1024

#define CLIENTHELLO "hello server"
#define SERVEROK "server ok"

#define ERROR0 "Head error"
#define ERROR1 "use name or pass error"
#define ERROR3 "Fun error"



typedef struct
{
	char name[MAX_BUF_SIZE / 2];
	char pass[MAX_BUF_SIZE / 2];
}user;

typedef struct
{
	int callmode;
	long buflen;
	char pFuncName[255];
}CallHead;

class Listen
{
public:
	string m_strIp;
	SOCKET m_Socket;
	Listen() 
	{
		m_Socket = INVALID_SOCKET;
		

	}
};


bool WSAInit();

bool SocketInit(SOCKET *hSocket);

void SockAddrInit(SOCKADDR_IN *hSockAddr,bool IsServer = TRUE);

bool SOCKET_Select(SOCKET hSocket, int nTimeOut = 100, bool bRead = FALSE);

DWORD WINAPI ClientThreadProc(LPVOID lpParameter);

int SENDbuf(SOCKET m_sock, const char *str);