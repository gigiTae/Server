#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>

#include "Error.h"

#pragma comment(lib, "Ws2_32.lib")



int main()
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	MessageBox(NULL, L"���� �ʱ�ȭ ����", L"����", MB_OK);

	// socket()
	SOCKET tcp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (tcp_sock == INVALID_SOCKET) error::Quit(L"socket()");
	MessageBox(NULL, L"TCP ���� ���� ����", L"����", MB_OK);

	// closesocket()
	closesocket(tcp_sock);

	// ���� ����
	WSACleanup();

	return 0;
}