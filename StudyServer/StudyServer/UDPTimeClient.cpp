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

int main()
{
	int retval;

	// �����ʱ�ȭ
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
	serverAddress.sin_addr.s_addr = inet_addr("172.35.1.13");

	// ������ ��ſ� ����� ����
	SOCKADDR_IN peerAddress;
	int addressLength;
	const char* key = "Get Time";
	char buffer[BUFFER_SIZE];

	InputManager input;
	input.Initalize(GetConsoleWindow());
	while (true)
	{
		input.Update(0.f);

		if (input.IsKeyState(Key::A, KeyState::Tap))
		{
			retval = sendto(sock, key, strlen(key), 0,
				(SOCKADDR*)&serverAddress, sizeof(serverAddress));

			if (retval == SOCKET_ERROR)
			{
				error::Display(L"sendto()");
				continue;
			}

			addressLength = sizeof(peerAddress);
			retval = recvfrom(sock, buffer, strlen(buffer), 0,
				(SOCKADDR*)&peerAddress, &addressLength);

			if (retval == SOCKET_ERROR)
			{
				error::Display(L"recvfrom");
				continue;
			}

			if (memcmp(&peerAddress, &serverAddress, sizeof(peerAddress)))
			{
				printf("[����] �߸��� ������ �Դϴ�!\n");
				continue;
			}

			// ���� ������ ���
			buffer[retval] = '\0';
			printf("���� �ð��� %s", buffer);
		}
	}

	return 0;
}