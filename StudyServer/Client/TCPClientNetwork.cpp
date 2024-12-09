#include "TCPClientNetwork.h"

#include <spdlog/spdlog.h>

client::TCPClientNetwork::TCPClientNetwork()
	:ClientSocket(INVALID_SOCKET)
{
}

client::TCPClientNetwork::~TCPClientNetwork()
{
}

client::EClientErrorCode client::TCPClientNetwork::Initialize()
{
	EClientErrorCode error = EClientErrorCode::None;

	error = InitializeSocket();
	if (error != EClientErrorCode::None) return error;


	return EClientErrorCode::None;
}

void client::TCPClientNetwork::Finalize()
{
	CloseSocket();
	WSACleanup();
}

client::EClientErrorCode client::TCPClientNetwork::Connect(short Port, const char* ServerIP)
{
	auto error = ConnectSocket(Port, ServerIP);

	if (error == EClientErrorCode::None) spdlog::trace("Connect Server");

	return error;
}

client::EClientErrorCode client::TCPClientNetwork::Disconnect()
{
	EClientErrorCode error = CloseSocket();

	if (error == EClientErrorCode::None) spdlog::trace("Disconnect Server");

	return error;
}

bool client::TCPClientNetwork::IsConnected() const
{
	return ClientSocket != INVALID_SOCKET;
}

client::EClientErrorCode client::TCPClientNetwork::InitializeSocket()
{
	WSADATA wsa{};
	WSAStartup(MAKEWORD(2, 2), &wsa);

	return EClientErrorCode::None;
}

client::EClientErrorCode client::TCPClientNetwork::ConnectSocket(short Port, const char* ServerIP)
{
	if (ClientSocket == INVALID_SOCKET)
	{
		ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (ClientSocket == INVALID_SOCKET)
		{
			return EClientErrorCode::ClientSocketCreateFail;
		}
	}

	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(Port);
	serverAddress.sin_addr.s_addr = inet_addr(ServerIP);

	int retval = connect(ClientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) return EClientErrorCode::ClientSocketConnectFail;

	return EClientErrorCode::None;
}

client::EClientErrorCode client::TCPClientNetwork::CloseSocket()
{
	if (ClientSocket != INVALID_SOCKET)
	{
		closesocket(ClientSocket);
		ClientSocket = INVALID_SOCKET;
	}

	spdlog::trace("[TCPClientNetwork] close socket");

	return EClientErrorCode::None;
}

