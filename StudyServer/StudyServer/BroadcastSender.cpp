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

	// 브로드캐스팅 활성화 
	BOOL bEnable = true;
	retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
		(char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR) error::Quit(L"setsokopt");

	// 소켓 주소 구조체 초기화
	SOCKADDR_IN remoteAddress;
	ZeroMemory(&remoteAddress, sizeof(remoteAddress));
	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_port = htons(9000);
	remoteAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	// 데이터 통신에 사용할 변수
	char buffer[BUFFER_SIZE + 1];
	int length{};

	while (true)
	{
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buffer, BUFFER_SIZE + 1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		length = strlen(buffer);
		if (buffer[length - 1] == '\n')
			buffer[length - 1] = '\0';
		if (strlen(buffer) == 0)
			break;

		// 데이터 보내기
		retval = sendto(sock, buffer, strlen(buffer), 0,
			(SOCKADDR*)&remoteAddress, sizeof(remoteAddress));
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"sendto()");
			continue;
		}
		printf("%d바이트를 보냈습니다\n", retval);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}