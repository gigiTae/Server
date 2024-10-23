#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "uitll.h"
#pragma comment(lib, "Ws2_32.lib")

constexpr size_t BUFFER_SIZE = 512;

int main()
{
	int retavl;

	// ���� �ʱ�ȭ 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) error::Quit(L"socket()");

	// ��Ƽĳ��Ʈ TTL ����
	int ttl = 2;
	retavl = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char*)&ttl, sizeof(ttl));
	if (retavl == SOCKET_ERROR) error::Quit(L"setsockopt()");

	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN remoteaddr{};
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(9000);
	remoteaddr.sin_addr.s_addr = inet_addr("172.35.1.13");

	// ������ ��ſ� ����� ����
	char buffer[BUFFER_SIZE + 1];
	int len;

	// ��Ƽĳ��Ʈ ������ ������
	while (true)
	{
		// ������ �Է�
		printf("\n[���� ������]");
		if (fgets(buffer, BUFFER_SIZE + 1, stdin) == NULL)
			break;

		// '\n' ���� ���� 
		len = strlen(buffer);
		if (buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';
		if (strlen(buffer) == 0)
			break;

		// ������ ������ 
		retavl = sendto(sock, buffer, strlen(buffer), 0,
			(SOCKADDR*)&remoteaddr, sizeof(remoteaddr));
		if (retavl == SOCKET_ERROR)
		{
			error::Display(L"sendto()");
			continue;
		}

		printf("%d����Ʈ�� ���½��ϴ�.\n", retavl);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
	return 0;
}