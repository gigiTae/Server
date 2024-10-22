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

	// Ŭ���̾�Ʈ ���� ���
	addressLeghth = sizeof(clientAddress);
	getpeername(clientSock, (SOCKADDR*)&clientAddress, &addressLeghth);

	while (1)
	{
		// ������ �ޱ�
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

		// ���� ������ ���
		buffer[retval] = '\0';
		printf("[TCP/%s:%d] %s\n",
			inet_ntoa(clientAddress.sin_addr),
			ntohs(clientAddress.sin_port), buffer);

		// ������ ������
		retval = send(clientSock, buffer, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"send");
			break;
		}
	}

	closesocket(clientSock);

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ %d\n",
		inet_ntoa(clientAddress.sin_addr),
		ntohs(clientAddress.sin_port));

	return 0;
}

int main()
{
	int retval;

	// �����ʱ�ȭ
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

	// ������ ��ſ� ����� ����
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

		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientAddress.sin_addr),
			ntohs(clientAddress.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)clientSock, 0, &ThreadId);

		if (hThread == NULL)
		{
			printf("[����] ������ ���� ����\n");
		}
		else
			CloseHandle(hThread);
	}

	// closesocket()
	closesocket(listen_sock);
	
	// ���� ����
	WSACleanup();
	return 0;
}