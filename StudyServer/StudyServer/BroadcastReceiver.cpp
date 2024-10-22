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
	int retval;

	// 윈속초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) error::Quit(L"socket()");

	// bind()
	SOCKADDR_IN localAddress;
	ZeroMemory(&localAddress, sizeof(localAddress));
	localAddress.sin_family = AF_INET;
	localAddress.sin_port = htons(9000);
	localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(sock, (SOCKADDR*)&localAddress, sizeof(localAddress));
	if (retval == SOCKET_ERROR) error::Quit(L"bind()");

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peerAddress;
	int addressLength;
	char buffer[BUFFER_SIZE + 1];

	// 브로드캐스트 데이터 받기
	while (true)
	{
		addressLength = sizeof(peerAddress);
		retval = recvfrom(sock, buffer, BUFFER_SIZE, 0,
			(SOCKADDR*)&peerAddress, &addressLength);
		if (retval == SOCKET_ERROR) error::Display(L"recvfrom()");

		// 받은 데이터 출력
		buffer[retval] = '\0';
		printf("[UDP/%s:%d] %s\n", inet_ntoa(peerAddress.sin_addr),
			ntohs(peerAddress.sin_port), buffer);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}