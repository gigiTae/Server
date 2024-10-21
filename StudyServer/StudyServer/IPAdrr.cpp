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

	// 원래의 IP 주소 출력
	const char* ipAddress = "119.194.159.48";
	printf(" IP 주소 : %s\n", ipAddress);

	// inet_addr() 함수 연습
	printf("IP 주소 (변환 후) = 0x%x\n", inet_addr(ipAddress));

	// inet_ntoa() 함수 연습
	IN_ADDR temp;
	temp.s_addr = inet_addr(ipAddress);
	printf("IP 주소 (변환 후) = %s\n", inet_ntoa(temp));

	WSACleanup();
	return 0;
}