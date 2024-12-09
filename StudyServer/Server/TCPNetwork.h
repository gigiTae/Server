#pragma once

#include "INetWork.h"

#include <vector>
#include <deque>

namespace server
{
	class TCPNetwork : public INetWork
	{
	public:
		TCPNetwork();
		~TCPNetwork();

		ENetErrorCode Initialize(ServerConfig InConfig) override;
		void Finalize() override;
		void Update() override;
		ENetErrorCode SendData(const PacketInfo& Info) override;
		int GetClientSessionPoolSize() override;

	protected:
		ENetErrorCode InitializeServerSocket();
		ENetErrorCode BindListen(short Port);

		int CreateSessionPool(const int MaxClientCount);
		ENetErrorCode NewSession();
		void SetSockOption(const SOCKET Sock);
		void ConnectedSession(const int SessionIndex, const SOCKET Sock, const char* IP);
		void CloseSession(const ESocketCloseCase CloseCase, const SOCKET InSock, const int SessionIndex);

		ENetErrorCode SetNoneBlockSocket(const SOCKET Sock);

		void AddPacketQueue(const int SessionIndex, const short PacketID, const short BodySize, char* DataPos);

	protected:
		ServerConfig Config;
		SOCKET ServerSocket;
		SOCKET MaxSockFD = 0;

		fd_set Readfds;
		size_t ConnectedSessionCount = 0;
		size_t ConnectSeq = 0;

		std::vector<ClientSession> ClientSessionPool;
		std::deque<RecvPacketInfo> PacketQueue;

	};


}