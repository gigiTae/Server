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

	u_short x = 0x1234;
	u_long y = 0x12345678;
	u_short x2;
	u_long y2;

	// ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ
	printf("ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ\n");
	printf("0x%x -> 0x%x\n", x, x2 = htons(x));
	printf("0x%x -> 0x%x\n", y, y2 = htons(y));

	// ��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ
	printf("��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ\n");
	printf("0x%x -> 0x%x\n", x2, ntohs(x2));
	printf("0x%x -> 0x%x\n", y2, ntohs(y2));



	WSACleanup();
	return 0;
}