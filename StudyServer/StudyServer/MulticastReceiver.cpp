#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "uitll.h"
#pragma comment(lib, "Ws2_32.lib")

constexpr size_t BUFFER_SIZE = 512; int main()
{
	int retavl;

	// ���� �ʱ�ȭ 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) error::Quit(L"socket()");

	// SO_REUSEADDR �ɼ� ����
	bool optval = true;
	retavl = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (retavl == SOCKET_ERROR) error::Quit(L"setsockopt()");

	// bind
	SOCKADDR_IN local_address;
	ZeroMemory(&local_address, sizeof(local_address));
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(9000);
	local_address.sin_addr.s_addr = htonl(INADDR_ANY);
	retavl = bind(sock, (SOCKADDR*)&local_address, sizeof(local_address));
	if (retavl == SOCKET_ERROR) error::Quit(L"bind()");

	// ��Ƽĳ��Ʈ �׷� ����
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retavl = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));
	if (retavl == SOCKET_ERROR) error::Quit(L"setsockopt()");

	// �����Ϳ� ����� ����
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buffer[BUFFER_SIZE + 1];

	// ��Ƽĳ��Ʈ ������ �ޱ�
	while (true)
	{
		// ������ �ޱ�
		addrlen = sizeof(peeraddr);
		retavl = recvfrom(sock, buffer, BUFFER_SIZE, 0
			, (SOCKADDR*)&peeraddr, &addrlen);
		if (retavl == SOCKET_ERROR)
		{
			error::Display(L"recvfrom()");
			continue;
		}
	
		//���� ������ ���
		buffer[retavl] = '\0';
		printf("data ->  %s \n", buffer);
	}

	// ��Ƽ ĳ��Ʈ �׷� Ż��
	retavl = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));
	if (retavl == SOCKET_ERROR) error::Quit(L"setsockopt()");


	closesocket(sock);
	WSACleanup();
	return 0;
}