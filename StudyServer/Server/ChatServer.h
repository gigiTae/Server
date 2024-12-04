#pragma once

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

namespace server
{
	class ChatServer
	{
	public:
		ChatServer() = default;
		~ChatServer() = default;

		bool Initialize();
		void Finalize();
		
		void ProcessConnect();

	private:
		SOCKET mListenSocket;
		SOCKADDR_IN mServerAddress;

	};


}