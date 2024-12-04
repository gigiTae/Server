#pragma once

#include <ws2tcpip.h> 
#include <winsock2.h>

struct ClientInfo
{
	SOCKET Socket;
	SOCKADDR_IN Address;
};

