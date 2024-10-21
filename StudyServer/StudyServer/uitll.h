#pragma once

#include <winsock2.h>
#include "Error.h"

namespace util
{
	// 도메인 이름 -> IP 주소 
	BOOL GetIPAddress(const char* name, IN_ADDR* address)
	{
		HOSTENT* ptr = gethostbyname(name);
		if (ptr == NULL)
		{
			error::Display(L"getohstbyname()");
			return FALSE;
		}

		memcpy(address, ptr->h_addr, ptr->h_length);
		return TRUE;
	}


	// IP 주소 -> 도메인 이름 
	BOOL GetDomainName(IN_ADDR address, char* name)
	{
		HOSTENT* ptr = gethostbyaddr(reinterpret_cast<char*>(&address)
			, sizeof(address)
			, AF_INET);

		if (ptr == NULL)
		{
			error::Display(L"gethostbyaddr()");
			return FALSE;
		}

		strcpy_s(name,  255 ,ptr->h_name);
		return TRUE;
	}


	int Recvn(SOCKET s, char* buf, int len, int flags)
	{
		int received;
		char* ptr = buf;
		int left = len;
		
		while (left > 0)
		{
			received = recv(s, ptr, left, flags);
			if (received == SOCKET_ERROR)
				return SOCKET_ERROR;
			else if (received == 0)
				break;

			left -= received;
			ptr += received;
		}
		return len - left;
	}
}