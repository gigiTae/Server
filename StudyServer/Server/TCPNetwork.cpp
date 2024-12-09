#include "TCPNetwork.h"

#include <spdlog/spdlog.h>

server::TCPNetwork::TCPNetwork()
{
}

server::TCPNetwork::~TCPNetwork()
{
}

server::ENetErrorCode server::TCPNetwork::Initialize(ServerConfig InConfig)
{
	Config = InConfig;

	ENetErrorCode error = ENetErrorCode::None;

	error = InitializeServerSocket();
	if (error != ENetErrorCode::None) return error;

	error = BindListen(Config.Port);
	if (error != ENetErrorCode::None) return error;

	FD_ZERO(&Readfds);
	FD_SET(ServerSocket, &Readfds);

	int poolSize = CreateSessionPool(Config.MaxClientCount + Config.ExtraClientCount);

	return ENetErrorCode::None;
}

server::ENetErrorCode server::TCPNetwork::InitializeServerSocket()
{
	WSADATA wsaData{};
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		return ENetErrorCode::SERVER_SOCKET_CREATE_FAIL;
	}

	int n = 1;

	// 서버 소켓을 특정 포드에 빠르게 재바인딩할 수 있도록 옵션을 설정
	if (setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&n, sizeof(n)) < 0)
	{
		return ENetErrorCode::SERVER_SOCKET_SO_REUSEADDR_FAIL;
	}

	spdlog::trace("[TCPNetWork] initialize server socket");

	return ENetErrorCode::None;
}

server::ENetErrorCode server::TCPNetwork::BindListen(short port)
{
	SOCKADDR_IN serverAddress{};

	serverAddress.sin_family = sizeof(serverAddress);
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServerSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		return ENetErrorCode::SERVER_SOCKET_BIND_FAIL;
	}

	if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		return ENetErrorCode::SERVER_SOCKET_LISTEN_FAIL;
	}

	spdlog::trace("[TCPNetWork] bind listen");

	return ENetErrorCode::None;
}

int server::TCPNetwork::CreateSessionPool(const int MaxClientCount)
{
	for (int i = 0; i < MaxClientCount; ++i)
	{
		ClientSession session;
		session.Index = i;
		session.RecvBuffer = new char[Config.MaxClientRecvBufferSize];
		session.SendBuffer = new char[Config.MaxClientSendBufferSize];
		ClientSessionPool.push_back(session);
	}

	return MaxClientCount;
}

server::ENetErrorCode server::TCPNetwork::NewSession()
{
	SOCKADDR_IN clientAddress{};
	int adrrLen = sizeof(clientAddress);
	SOCKET clientSock = accept(ServerSocket, (SOCKADDR*)&clientAddress, &adrrLen);
	if (clientSock == INVALID_SOCKET)
	{
		return ENetErrorCode::SERVER_SOCKET_ACCEPT_FAIL;
	}

	char clientIP[MAX_IP_LEN] = { 0, };
	inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, MAX_IP_LEN - 1);
	SetSockOption(clientSock);
	SetNoneBlockSocket(clientSock);
	FD_SET(clientSock, &Readfds);
	ConnectedSession(adrrLen, clientSock, clientIP);
	spdlog::trace("[TCPNetWork] new session {} {} {}", clientSock, ConnectSeq, clientIP);

	return ENetErrorCode::None;
}

void server::TCPNetwork::SetSockOption(const SOCKET Sock)
{
	linger ling;
	ling.l_onoff = 0; // 옵션의 활성화 여부 (0:비활성화 , 1:활성화)
	ling.l_linger = 0; // 소켓을 닫을 때 남아 있는 데이터 전송을 시도하는 대기 시간(초)
	setsockopt(Sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));

	// 수신/송신 버퍼 크기를 사용자 정의 값으로 설정
	int recvSize = Config.MaxClientSockOptRecvBufferSize;
	int sendSize = Config.MaxClientSockOptSendBufferSize;
	setsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char*)&recvSize, sizeof(recvSize));
	setsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char*)&sendSize, sizeof(sendSize));
}

void server::TCPNetwork::ConnectedSession(const int SessionIndex, const SOCKET Sock, const char* IP)
{
	if (MaxSockFD < Sock)
	{
		MaxSockFD = Sock;
	}

	++ConnectSeq;
	++ConnectedSessionCount;

	auto& session = ClientSessionPool[SessionIndex];
	session.Seq = ConnectSeq;
	session.SocketFD = Sock;
	memcpy(session.IP, IP, MAX_IP_LEN - 1);

	AddPacketQueue(SessionIndex, 0, 0, nullptr);
}

void server::TCPNetwork::CloseSession(const ESocketCloseCase CloseCase, const SOCKET InSock, const int SessionIndex)
{
	if (CloseCase == ESocketCloseCase::SessionPoolEmpty)
	{
		closesocket(InSock);
		FD_CLR(InSock, &Readfds);
		return;
	}

}

server::ENetErrorCode server::TCPNetwork::SetNoneBlockSocket(const SOCKET Sock)
{
	unsigned long mode = 1;

	if (ioctlsocket(ServerSocket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		return ENetErrorCode::SERVER_SOCKET_FIONBIO_FAIL;
	}

	return ENetErrorCode::None;
}

void server::TCPNetwork::AddPacketQueue(const int SessionIndex, const short PacketID, const short BodySize, char* Data)
{
	RecvPacketInfo info;
	info.SessionIndex = SessionIndex;
	info.PacketID = PacketID;
	info.PacketBodySize = BodySize;
	info.Data = Data;
	PacketQueue.push_back(info);
}


void server::TCPNetwork::Finalize()
{
	WSACleanup();
}

server::ENetErrorCode server::TCPNetwork::SendData(const PacketInfo& Info)
{
	return ENetErrorCode();
}

int server::TCPNetwork::GetClientSessionPoolSize()
{
	return 0;
}


void server::TCPNetwork::Update()
{
	fd_set readSet = Readfds;
	fd_set writeSet = Readfds;

	timeval timeout{ 0, 1000 }; // tv_sec, tv_usec

	int selectResult = select(0, &readSet, &writeSet, 0, &timeout);


	if (FD_ISSET(ServerSocket, &readSet))
	{
		NewSession();
	}





}
