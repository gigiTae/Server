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
	serverAddress.sin_addr.s_addr = inet_addr("121.165.95.167");

	// ������ ��ſ� ����� ����
	SOCKADDR_IN peerAddress;
	int addressLength;
	char buffer[BUFFER_SIZE + 1];
	int length;
	
	// ������ ������ ���
	while (true)
	{
		printf("\n[���� ������] ");
		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
			break;

		// '\n' ���� ����
		length = strlen(buffer);
		if (buffer[length - 1] == '\n')
			buffer[length - 1] = '\0';
		if(strlen(buffer) == 0)
			break;

		// ������ ������
		retval = sendto(sock, buffer, strlen(buffer), 0,
			(SOCKADDR*)&serverAddress, sizeof(serverAddress));

		if (retval == SOCKET_ERROR)
		{
			error::Display(L"sendto()");
			continue;
		}
		printf("[UDP Client] %d ����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		addressLength = sizeof(peerAddress);
		retval = recvfrom(sock, buffer, BUFFER_SIZE, 0
			, (SOCKADDR*)&peerAddress, &addressLength);

		if (retval == SOCKET_ERROR)
		{
			error::Display(L"recvfrom()");
			continue;
		}

		// �۽����� IP �ּ� üũ
		if (memcmp(&peerAddress, &serverAddress, sizeof(peerAddress)))
		{
			printf("[����] �߸��� ������ �Դϴ�!\n");
			continue;
		}

		// ���� ������ ���
		buffer[retval] = '\0';
		printf("[UDP Ŭ���̾�Ʈ] %d ����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buffer);
	}


	closesocket(sock);
	WSACleanup();
	return 0;
}