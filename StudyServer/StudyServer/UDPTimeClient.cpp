#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "Error.h"
#include "uitll.h"
#include "InputManager.h"
#pragma comment(lib, "Ws2_32.lib")


constexpr size_t BUFFER_SIZE = 512;

void RequsetThread()
{
	// Input 초기화 
	InputManager input{};
	input.Initalize(GetConsoleWindow());

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) error::Quit(L"socket()");

	// bind()
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = inet_addr("121.165.95.167");

	const char* buffer = "input";
	while (true)
	{
		input.Update(0.f);
		if (input.IsKeyState(Key::A, KeyState::Tap))
		{
			printf("Requset\n");

			int retavl = sendto(sock, buffer, strlen(buffer), 0,
				(SOCKADDR*)&serverAddress, sizeof(serverAddress));
			if (retavl == SOCKET_ERROR)
			{
				error::Display(L"sendto()");
				continue;
			}
		}
	}
}

int main()
{
	// Input 초기화 
	InputManager input{};
	input.Initalize(GetConsoleWindow());

	int retval{};

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	std::thread t1(RequsetThread);

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

	SOCKADDR_IN peerAddress;
	int addressLength{};
	char buffer[BUFFER_SIZE + 1]{};

	while (true)
	{
		addressLength = sizeof(peerAddress);
		retval = recvfrom(sock, buffer, BUFFER_SIZE, 0,
			(SOCKADDR*)&peerAddress, &addressLength);
		if (retval == SOCKET_ERROR) error::Display(L"recvfrom()");
		else
		{
			buffer[retval] = '\0';
			printf("[UDP] %s\n", buffer);
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}