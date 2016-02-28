#include "Log.h"
#include "socket.h"
#include <WS2tcpip.h>

bool WSAInit()
{
	WSADATA wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR) {
		return true;
	}
	else {
		return false;
	}
}

bool SocketInit(SOCKET *hSocket)
{
	if ((*hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		return false;
	return  true;
	
}

void SockAddrInit(SOCKADDR_IN *hSockAddr, bool IsServer )
{
	if (IsServer == FALSE)
	{
		(*hSockAddr).sin_addr.S_un.S_addr = inet_addr(INTADDR);
	}
	else
	{
		(*hSockAddr).sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	}
	
	(*hSockAddr).sin_family = AF_INET;
	(*hSockAddr).sin_port = htons(PORT);
	
}

bool SOCKET_Select(SOCKET hSocket, int nTimeOut, bool bRead)
{
	fd_set fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket, &fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;

	int iRet = 0;
	if (bRead) {
		iRet = select(0, &fdset, NULL, NULL, &tv);
	}
	else {
		iRet = select(0, NULL, &fdset, NULL, &tv);
	}

	if (iRet <= 0) {
		return false;
	}
	else if (FD_ISSET(hSocket, &fdset)) {
		return true;
	}
	return false;
}


/*发送字符串*/
int SENDbuf(SOCKET m_sock, const char *str)
{
	char sendBuf[MAX_BUF_SIZE] = { 0 };
	strcpy(sendBuf, str);
	return send(m_sock, sendBuf, MAX_BUF_SIZE, 0);
}



//int startFunc() {
//	
//	SOCKET S_SO, new_C_SO;
//	SOCKADDR_IN S_Adr, C_Adr;
//	int port = 789;
//	int C_AdrLen = sizeof(C_Adr);
//	char sendbuf[200] = "hello client!";
//	char recvbuff[200];
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0)
//	{
//		Log("startup error!");
//		return 0;
//	}
//
//	S_SO = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (S_SO == INVALID_SOCKET)
//	{
//		Log("S_socket error!");
//		return 0;
//	}
//	S_Adr.sin_family = AF_INET;
//	S_Adr.sin_port = htons(port);
//	S_Adr.sin_addr.s_addr = htonl(INADDR_ANY);
//
//	if (bind(S_SO, (SOCKADDR *)&S_Adr, sizeof(S_Adr)) == SOCKET_ERROR)
//	{
//		Log("bind");
//		return 0;
//	}
//
//
//	if (listen(S_SO, 5) == SOCKET_ERROR)
//	{
//		Log("listen error!");
//		return 0;
//	}
//
//	while (true)
//	{
//		new_C_SO = accept(S_SO, (SOCKADDR *)&C_Adr, &C_AdrLen);
//		if (new_C_SO == INVALID_SOCKET)
//		{
//			Log("accept error!\n");
//			continue;
//		}
//		Log("第一个链接: %s \n", inet_ntoa(C_Adr.sin_addr));
//
//		int ret = recv(new_C_SO, recvbuff, 100, 0);
//		if (ret > 0)
//		{
//			recvbuff[ret] = 0x00;
//			Log("客户端：%s \n", recvbuff);
//
//		}
//		if (strcmp(recvbuff, "hello serve!") == 0)
//		{
//			char sendbuf[200] = "hello client!";
//			Log("send 开始发送\n");
//			send(new_C_SO, sendbuf, sizeof(sendbuf), 0);
//			Log("服务端：%s \n", sendbuf);
//			break;
//		}
//	}
//