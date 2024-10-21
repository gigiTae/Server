#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>

#include "Error.h"

#pragma comment(lib, "Ws2_32.lib")



int main()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	MessageBox(NULL, L"윈속 초기화 성공", L"성공", MB_OK);

	// socket()
	SOCKET tcp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (tcp_sock == INVALID_SOCKET) error::Quit(L"socket()");
	MessageBox(NULL, L"TCP 소켓 생성 성공", L"성공", MB_OK);

	// closesocket()
	closesocket(tcp_sock);

	// 윈속 종료
	WSACleanup();

	return 0;
}