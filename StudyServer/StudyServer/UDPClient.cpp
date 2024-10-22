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
	serverAddress.sin_addr.s_addr = inet_addr("121.165.95.167");

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peerAddress;
	int addressLength;
	char buffer[BUFFER_SIZE + 1];
	int length;
	
	// 서버와 데이터 통신
	while (true)
	{
		printf("\n[보낼 데이터] ");
		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
			break;

		// '\n' 문자 제거
		length = strlen(buffer);
		if (buffer[length - 1] == '\n')
			buffer[length - 1] = '\0';
		if(strlen(buffer) == 0)
			break;

		// 데이터 보내기
		retval = sendto(sock, buffer, strlen(buffer), 0,
			(SOCKADDR*)&serverAddress, sizeof(serverAddress));

		if (retval == SOCKET_ERROR)
		{
			error::Display(L"sendto()");
			continue;
		}
		printf("[UDP Client] %d 바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		addressLength = sizeof(peerAddress);
		retval = recvfrom(sock, buffer, BUFFER_SIZE, 0
			, (SOCKADDR*)&peerAddress, &addressLength);

		if (retval == SOCKET_ERROR)
		{
			error::Display(L"recvfrom()");
			continue;
		}

		// 송신자의 IP 주소 체크
		if (memcmp(&peerAddress, &serverAddress, sizeof(peerAddress)))
		{
			printf("[오류] 잘못된 데이터 입니다!\n");
			continue;
		}

		// 받은 데이터 출력
		buffer[retval] = '\0';
		printf("[UDP 클라이언트] %d 바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buffer);
	}


	closesocket(sock);
	WSACleanup();
	return 0;
}