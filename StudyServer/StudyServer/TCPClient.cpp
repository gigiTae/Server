#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "uitll.h"
#pragma comment(lib, "Ws2_32.lib")

constexpr size_t BUFSIZE = 512;

int main()
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) error::Quit(L"socket()");

	// connect()
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = inet_addr("121.165.95.167");
	retval = connect(sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) error::Quit(L"connect()");

	
	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (1)
	{
		// ������ �Է�
		ZeroMemory(buf, sizeof(buf));
		printf("\n[send data]");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		// '\n' ���� ����
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if(strlen(buf) == 0)
			break;

		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"send()");
			break;
		}
		printf("[TCP client] %d byte send\n", retval);

		// ������ �ޱ�
		retval = recv(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"recv()");
			break;
		}
		// ������ ���� ����� ���
		else if(retval ==0) 
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP client] %d byte get \n", retval);
		printf("[get data] %s\n", buf);
	}

	// closesocket()
	closesocket(sock);

	//���� ����
	WSACleanup();
	return 0;
}






