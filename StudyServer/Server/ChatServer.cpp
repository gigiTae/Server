#include "ChatServer.h"

#include <spdlog/spdlog.h>

#include "../Common/ThreadPool.h"

bool server::ChatServer::Initialize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// socket
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		spdlog::warn("[ChatServer] sokcet() fail");
		return false;
	}

	// bind
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(9000);
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int retval = bind(ListenSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("[ChatServer] bind() fail");
		return false;
	}

	// listen
	retval = listen(ListenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("[ChatServer] listen() fail");
		return false;
	}

	common::ThreadPool::Get()->EnqueueJob([this]()
		{
			ProcessConnectClient();
		});
}

void server::ChatServer::Finalize()
{

}

void server::ChatServer::ProcessConnectClient()
{
	while (!bIsEnd)
	{
		SOCKET clientSock;
		SOCKADDR_IN clientAddress;
		int addrLen;
		addrLen = sizeof(clientAddress);

		clientSock = accept(ListenSocket, (SOCKADDR*)&clientAddress, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			spdlog::error("accept");
			continue;
		}

		char ipstr[INET_ADDRSTRLEN]{};
		inet_ntop(AF_INET, &(clientAddress.sin_addr), ipstr, INET_ADDRSTRLEN);

		int portNum = ntohs(clientAddress.sin_port);

		spdlog::info("connect client [ip] %s [port] %d", ipstr, portNum);
	
		std::unique_lock lock(Mutex);
		ClientInfos.push_back({ clientSock, clientAddress });
	}
}

