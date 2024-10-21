#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "uitll.h"
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	IN_ADDR addr;
	if (util::GetIPAddress("www.naver.com", &addr))
	{
		// 성공이면 결과 출력
		printf("IP 주소 = %s\n", inet_ntoa(addr));

		// IP 주소 -> 도메인 이름
		char name[256];
		if (util::GetDomainName(addr, name))
		{
			// 성공이면 결과 출력
			printf("도메인 이름 = %s\n", name);
		}
	}

	WSACleanup();
	return 0;
}