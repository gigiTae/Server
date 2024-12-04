#pragma once

#include <ws2tcpip.h> 
#include <winsock2.h>
#include <vector>
#include <mutex>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#include "ClientInfo.h"

namespace server
{
	class ChatServer
	{
	public:
		ChatServer() = default;
		~ChatServer() = default;

		bool Initialize();
		void Finalize();
		
		/// <summary>
		/// Ŭ���̾�Ʈ ������ ó��
		/// </summary>
		void ProcessConnectClient();

	private:
	

		bool bIsEnd;

		SOCKET ListenSocket;
		SOCKADDR_IN ServerAddress;

		std::vector<ClientInfo> ClientInfos;

		std::mutex Mutex;
	};


}