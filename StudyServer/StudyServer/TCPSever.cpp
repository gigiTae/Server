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

	// SO_REUSEADDR 소켓 옵션 설정
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

	// 데이터 통신에 사용할 변수
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

		char ipstr[INET_ADDRSTRLEN]; // IPv4를 위한 버퍼 크기 (IPv6를 위해서는 INET
		inet_ntop(AF_INET, &(clientAddress.sin_addr), ipstr, INET_ADDRSTRLEN);

		printf("\n[TCP 서버] 클라이언트 접속 : IP = %s, 포트 번호=%d\n",
			ipstr,
			ntohs(clientAddress.sin_port));

		// 클라이언트와 데이터 통신
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

			// 받은 데이터 출력
			buf[retval] = '\0';
			char ipstr[INET_ADDRSTRLEN]; // IPv4를 위한 버퍼 크기 (IPv6를 위해서는 INET
			inet_ntop(AF_INET, &(clientAddress.sin_addr), ipstr, INET_ADDRSTRLEN);

			printf("[TCP/%s:%d] %s\n",
				ipstr,
				ntohs(clientAddress.sin_port), buf);

			// 데이터 보내기
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR)
			{
				error::Display(L"send()");
				break;
			}
		}
		// closesocket()
		closesocket(client_sock);

		char ipstr2[INET_ADDRSTRLEN]; // IPv4를 위한 버퍼 크기 (IPv6를 위해서는 INET
		inet_ntop(AF_INET, &(clientAddress.sin_addr), ipstr2, INET_ADDRSTRLEN);

		printf("[TCP server] client end : ip address=%s, port number=%d\n",
			ipstr2,
			ntohs(clientAddress.sin_port));
	}
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}






