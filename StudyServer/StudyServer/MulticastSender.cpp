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

	// 윈속 초기화 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) error::Quit(L"socket()");

	// 멀티캐스트 TTL 설정
	int ttl = 2;
	retavl = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char*)&ttl, sizeof(ttl));
	if (retavl == SOCKET_ERROR) error::Quit(L"setsockopt()");

	// 소켓 주소 구조체 초기화
	SOCKADDR_IN remoteaddr{};
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(9000);
	remoteaddr.sin_addr.s_addr = inet_addr("172.35.1.13");

	// 데이터 통신에 사용할 변수
	char buffer[BUFFER_SIZE + 1];
	int len;

	// 멀티캐스트 데이터 보내기
	while (true)
	{
		// 데이터 입력
		printf("\n[보낼 데이터]");
		if (fgets(buffer, BUFFER_SIZE + 1, stdin) == NULL)
			break;

		// '\n' 문자 제거 
		len = strlen(buffer);
		if (buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';
		if (strlen(buffer) == 0)
			break;

		// 데이터 보내기 
		retavl = sendto(sock, buffer, strlen(buffer), 0,
			(SOCKADDR*)&remoteaddr, sizeof(remoteaddr));
		if (retavl == SOCKET_ERROR)
		{
			error::Display(L"sendto()");
			continue;
		}

		printf("%d바이트를 보냈습니다.\n", retavl);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
	return 0;
}