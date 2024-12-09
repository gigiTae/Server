#pragma once

#include <winsock2.h>
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib")

#include "Define.h"
#include "ServerErrorCode.h"


namespace server
{
	class INetWork
	{
	public:
		virtual ~INetWork() {}

		virtual ENetErrorCode Initialize(ServerConfig InConfig) abstract;

		virtual void Update() abstract;
		
		virtual void Finalize() abstract;

		virtual ENetErrorCode SendData(const PacketInfo& Info) abstract;

		virtual int GetClientSessionPoolSize() abstract;
	};


}