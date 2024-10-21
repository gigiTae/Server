#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"

#pragma comment(lib, "Ws2_32.lib")


int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// ������ IP �ּ� ���
	const char* ipAddress = "119.194.159.48";
	printf(" IP �ּ� : %s\n", ipAddress);

	// inet_addr() �Լ� ����
	printf("IP �ּ� (��ȯ ��) = 0x%x\n", inet_addr(ipAddress));

	// inet_ntoa() �Լ� ����
	IN_ADDR temp;
	temp.s_addr = inet_addr(ipAddress);
	printf("IP �ּ� (��ȯ ��) = %s\n", inet_ntoa(temp));

	WSACleanup();
	return 0;
}