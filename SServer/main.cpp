#include "WinService.h"
#include "socket.h"
#include "Log.h"
#include "runfun.h"

#include <fstream>

#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <atlstr.h>
#pragma comment(lib, "Ws2_32.lib")
//
using namespace std;
void MainService()
{
	SOCKET ListenSock;
	SOCKADDR_IN ListenAddr;

	if (WSAInit() == FALSE)
	{
		Log("WSA ERROR");
		goto __Error_End;
	}
	if (SocketInit(&ListenSock) == FALSE)
	{
		Log("Listen Socket Init ERROR");
		goto __Error_End;
	}
	
	SockAddrInit(&ListenAddr);

	if (bind(ListenSock, (SOCKADDR *)&ListenAddr, sizeof(ListenAddr)) == SOCKET_ERROR)
	{
		Log("bind");
		goto __Error_End;
	}

	if (listen(ListenSock, 5) == SOCKET_ERROR)
	{
		Log("listen error!");
		goto __Error_End;
	}
	while (TRUE)
	{
		if (SOCKET_Select(ListenSock, 1000, TRUE))
		{
			sockaddr_in clientAddr;
			int iLen = sizeof(sockaddr_in);
			SOCKET accSock = accept(ListenSock, (struct sockaddr *)&clientAddr, &iLen);
			if (accSock == INVALID_SOCKET)
			{
				continue;
			}
			Listen ls;
			ls.m_Socket = accSock;
			ls.m_strIp=inet_ntoa(clientAddr.sin_addr);
			Log(ls.m_strIp);
			_beginthread((_beginthread_proc_type)ClientThreadProc,0,&ls);
		}
	}
	

__Error_End:
	closesocket(ListenSock);
	
	return ;

}
/*从数据库中读取用户信息对比*/
bool readPass(user us)
{
	char name[MAX_BUF_SIZE / 2];
	char pass[MAX_BUF_SIZE / 2];
	fstream fs;
	fs.open("user.lib", ios::in);
	if (!fs)return true;
	fs >> name ;
	fs >> pass;
	fs.close();
	if (!strcmp(us.name, name) && !strcmp(us.pass, pass))
	{
		return true;
	}
	return false;
}

void waitsocket(SOCKET m_sock)
{
	while (!SOCKET_Select(m_sock, 100, TRUE))Sleep(500);
}


/*接收用户信息*/
inline bool RecvUserInfo(Listen ls)
{
		char recvBuf[MAX_BUF_SIZE] = { 0 };
		char sendBuf[MAX_BUF_SIZE] = { 0 };
	recvHEAD:
		if (recv(ls.m_Socket, recvBuf, MAX_BUF_SIZE, 0) > 0)
		{
			if (strcmp(recvBuf, CLIENTHELLO))
			{

				SENDbuf(ls.m_Socket, ERROR0);
				while (!SOCKET_Select(ls.m_Socket, 100, TRUE))Sleep(500);
				goto recvHEAD;
			}
			SENDbuf(ls.m_Socket, SERVEROK);

		recvPASS:
			while (!SOCKET_Select(ls.m_Socket, 100, TRUE))Sleep(500);
			if (recv(ls.m_Socket, recvBuf, MAX_BUF_SIZE, 0))
			{
				user us = *(user *)&recvBuf;
				if (!readPass(us))
				{
					SENDbuf(ls.m_Socket, ERROR1);
					
					goto recvPASS;
				}
				SENDbuf(ls.m_Socket, SERVEROK);
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;	
		
}
DWORD WINAPI ClientThreadProc(LPVOID lpParameter)
{
	Listen ls = *(Listen *)lpParameter;

	while (true)
	{
		if (SOCKET_Select(ls.m_Socket, 100, TRUE))
		{
			if (RecvUserInfo(ls))
			{
				while (true)
				{
					if (SOCKET_Select(ls.m_Socket, 100, TRUE))
					{
						CallHead head;
						if (recv(ls.m_Socket, (char *)&head, sizeof(CallHead), 0))
						{
							char *reBuf = new char[head.buflen];
							waitsocket(ls.m_Socket);
							if (recv(ls.m_Socket, reBuf, head.buflen, 0))
							{
								/*处理字符串*/
								switch (head.callmode)
								{
								case 1:FuncSMode1(ls,head, reBuf); break;
								case 2:FuncSMode2(ls,head, reBuf); break;
								case 3:FuncSMode3(ls,head, reBuf); break;
								default:/*错误处理*/;

								}


							}
							else goto SOCKEND;

							delete[] reBuf;
						}
						else goto SOCKEND;
					}
					Sleep(500);
				}
			}
			else
			{
				string str;
				str = ls.m_strIp;
				str	+" 断开连接";
				Log(str);

				break;
			}
		}
		Sleep(500);
	}

SOCKEND:
	closesocket(ls.m_Socket);
	return TRUE;
	//CString strMsg;
	//
	//while (TRUE && !(m_ClientItem.m_pMainWnd->bShutDown)) {
	//	if (SOCKET_Select(m_ClientItem.m_Socket, 100, TRUE)) {
	//		TCHAR szBuf[MAX_BUF_SIZE] = { 0 };
	//		int iRet = recv(m_ClientItem.m_Socket, (char *)szBuf, MAX_BUF_SIZE, 0);
	//		if (iRet > 0) {
	//			//right;
	//			strMsg = szBuf;
	//			strMsg = _T("客户端：") + m_ClientItem.m_strIp + _T(">") + strMsg;
	//			m_ClientItem.m_pMainWnd->ShowMsg(strMsg);
	//			m_ClientItem.m_pMainWnd->SendClientsMsg(strMsg, &m_ClientItem);
	//		}
	//		else {
	//			//close socket;
	//			strMsg = _T("客户端：") + m_ClientItem.m_strIp + _T(" 离开了聊天室！");
	//			m_ClientItem.m_pMainWnd->ShowMsg(strMsg);
	//			m_ClientItem.m_pMainWnd->RemoveClientFromArray(m_ClientItem);
	//			m_ClientItem.m_pMainWnd->SendClientsMsg(strMsg, &m_ClientItem);
	//			break;
	//		}
	//	}
	//	Sleep(500);
	//}
	//return TRUE;
}
