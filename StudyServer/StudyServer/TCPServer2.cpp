#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "uitll.h"
#pragma comment(lib, "Ws2_32.lib")

constexpr size_t BUFFER_SIZE = 512;

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET clientSock = (SOCKET)arg;
	char buffer[BUFFER_SIZE + 1];
	SOCKADDR_IN clientAddress;
	int addressLeghth;
	int retval;

	// 클라이언트 정보 얻기
	addressLeghth = sizeof(clientAddress);
	getpeername(clientSock, (SOCKADDR*)&clientAddress, &addressLeghth);

	while (1)
	{
		// 데이터 받기
		retval = recv(clientSock, buffer, BUFFER_SIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		// 받은 데이터 출력
		buffer[retval] = '\0';
		printf("[TCP/%s:%d] %s\n",
			inet_ntoa(clientAddress.sin_addr),
			ntohs(clientAddress.sin_port), buffer);

		// 데이터 보내기
		retval = send(clientSock, buffer, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"send");
			break;
		}
	}

	closesocket(clientSock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호 %d\n",
		inet_ntoa(clientAddress.sin_addr),
		ntohs(clientAddress.sin_port));

	return 0;
}

int main()
{
	int retval;

	// 윈속초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) error::Quit(L"socket()");

	// bind
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) error::Quit(L"bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) error::Quit(L"listen()");

	// 데이터 통신에 사용할 변수
	SOCKET clientSock;
	SOCKADDR_IN clientAddress;
	int addressLength;
	HANDLE hThread;
	DWORD ThreadId;

	while (true)
	{
		//accept()
		addressLength = sizeof(clientAddress);
		clientSock = accept(listen_sock, (SOCKADDR*)&clientAddress, &addressLength);
		if (clientSock == INVALID_SOCKET)
		{
			error::Display(L"accept()");
			continue;
		}

		printf("[TCP 서버] 클라이언트 접속: IP=%s, 포트 번호=%d\n",
			inet_ntoa(clientAddress.sin_addr),
			ntohs(clientAddress.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)clientSock, 0, &ThreadId);

		if (hThread == NULL)
		{
			printf("[오류] 스레드 생성 실패\n");
		}
		else
			CloseHandle(hThread);
	}

	// closesocket()
	closesocket(listen_sock);
	
	// 윈속 종료
	WSACleanup();
	return 0;
}