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
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) error::Quit(L"socket()");

	// SO_REUSEADDR ���� �ɼ� ����
	bool optval = true;
	retval = setsockopt(listen_sock, SOL_SOCKET,
		SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) error::Quit(L"setsockopt()");
		 
		// bind()
		SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) error::Quit(L"bind");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) error::Quit(L"listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientAddress;
	int addr_length;
	char buf[BUFSIZE + 1];

	while (1)
	{
		// accept ()
		addr_length = sizeof(clientAddress);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientAddress, &addr_length);
		if (client_sock == INVALID_SOCKET)
		{
			error::Display(L"accept()");
			continue;
		}

		char ipstr[INET_ADDRSTRLEN]; // IPv4�� ���� ���� ũ�� (IPv6�� ���ؼ��� INET
		inet_ntop(AF_INET, &(clientAddress.sin_addr), ipstr, INET_ADDRSTRLEN);

		printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP = %s, ��Ʈ ��ȣ=%d\n",
			ipstr,
			ntohs(clientAddress.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1)
		{
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR)
			{
				error::Display(L"recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			char ipstr[INET_ADDRSTRLEN]; // IPv4�� ���� ���� ũ�� (IPv6�� ���ؼ��� INET
			inet_ntop(AF_INET, &(clientAddress.sin_addr), ipstr, INET_ADDRSTRLEN);

			printf("[TCP/%s:%d] %s\n",
				ipstr,
				ntohs(clientAddress.sin_port), buf);

			// ������ ������
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR)
			{
				error::Display(L"send()");
				break;
			}
		}
		// closesocket()
		closesocket(client_sock);

		char ipstr2[INET_ADDRSTRLEN]; // IPv4�� ���� ���� ũ�� (IPv6�� ���ؼ��� INET
		inet_ntop(AF_INET, &(clientAddress.sin_addr), ipstr2, INET_ADDRSTRLEN);

		printf("[TCP server] client end : ip address=%s, port number=%d\n",
			ipstr2,
			ntohs(clientAddress.sin_port));
	}
	closesocket(listen_sock);

	//���� ����
	WSACleanup();
	return 0;
}






