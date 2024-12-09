#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib")

#include "ClientErrorCode.h"

namespace client
{
	class IClientNetwork
	{
	public:
		virtual ~IClientNetwork() {}
		virtual EClientErrorCode Initialize() abstract;
		virtual void Finalize() abstract;
		virtual EClientErrorCode Connect(short Port, const char* IPAddress) abstract;
		virtual EClientErrorCode Disconnect() abstract;
		virtual bool IsConnected() const abstract;

	};
}