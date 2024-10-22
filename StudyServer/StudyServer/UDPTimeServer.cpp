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
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) error::Quit(L"bind");

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN clientAddress;
	int addressLength;
	char buffer[BUFFER_SIZE + 1];

	while (true)
	{
		// 데이터 받기
		addressLength = sizeof(clientAddress);
		retval = recvfrom(sock, buffer, BUFFER_SIZE, 0,
			(SOCKADDR*)&clientAddress, &addressLength);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"recvfrom()");
			continue;
		}

		// 받은데이터 출력
		buffer[retval] = '\0';
		printf("[UDP/%s:%d] %s\n",
			inet_ntoa(clientAddress.sin_addr),
			ntohs(clientAddress.sin_port), buffer);
	}

	return 0;
}