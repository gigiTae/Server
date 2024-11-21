#include "ChatServer.h"

#include <spdlog/spdlog.h>

bool server::ChatServer::Initialize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// socket
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket == INVALID_SOCKET)
	{
		spdlog::warn("[ChatServer] sokcet() fail");
		return false;
	}

	// bind
	mServerAddress.sin_family = AF_INET;
	mServerAddress.sin_port = htons(9000);
	mServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int retval = bind(mListenSocket, (SOCKADDR*)&mServerAddress, sizeof(mServerAddress));
	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("[ChatServer] bind() fail");
		return false;
	}

	// listen
	retval = listen(mListenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("[ChatServer] listen() fail");
		return false;
	}
}

void server::ChatServer::Finalize()
{

}
