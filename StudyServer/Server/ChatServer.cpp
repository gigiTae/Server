#include "ChatServer.h"

#include <spdlog/spdlog.h>

#include "../Common/ThreadPool.h"
#include "../Common/PMessage.h"
#include "../Common/Utill.h"

bool server::ChatServer::Initialize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		spdlog::warn("[ChatServer] sokcet() fail");
		return false;
	}

	// bind
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(9000);
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int retval = bind(ServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("[ChatServer] bind() fail");
		return false;
	}

	// listen
	retval = listen(ServerSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("[ChatServer] listen() fail");
		return false;
	}

	common::ThreadPool::Get()->EnqueueJob([this]()
		{
			ProcessClientPacket();
		});


	common::ThreadPool::Get()->EnqueueJob([this]()
		{
			ProcessWorkList();
		});
}

void server::ChatServer::Finalize()
{
}

void server::ChatServer::ProcessClientPacket()
{
	fd_set readfds;
	int max_sd;
	timeval timeout;

	while (!bIsEnd)
	{
		FD_ZERO(&readfds);
		FD_SET(ServerSocket, &readfds);
		max_sd = ServerSocket;

		for (int i = 0; i < ClientSockets.size(); ++i)
		{
			SOCKET sd = ClientSockets[i];
			if (sd > 0) FD_SET(sd, &readfds);
			if (sd > max_sd) max_sd = sd;
		}

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int retval = select(max_sd + 1, &readfds, nullptr, nullptr, &timeout);

		if (retval < 0)
		{
			spdlog::warn("[CharServer] select() fail");
		}

		// 새로운 연결 요청
		if (FD_ISSET(ServerSocket, &readfds))
		{
			SOCKET clientSock;
			SOCKADDR_IN clientAddress;
			int addrLen = sizeof(clientAddress);
			clientSock = accept(ServerSocket, (SOCKADDR*)&clientAddress, &addrLen);

			if (clientSock < 0)
			{
				spdlog::warn("[CharServer] accept() fail");
			}

			spdlog::info("[CharServer] connect new client");

			std::unique_lock<std::mutex> sockLock(SocketMutex);
			ClientSockets.push_back(clientSock);

			std::unique_lock<std::mutex> workLock(WorkMutex);
			std::string str = "Connect Player";
			WorkList.push({ common::EPacketType::Message, str, (int)str.size() });
			WorkCV.notify_one();
		}

		// 클라이언트 데이터 처리 
		for (int i = 0; i < ClientSockets.size(); ++i)
		{
			SOCKET clientSock = ClientSockets[i];
			if (FD_ISSET(clientSock, &readfds))
			{
				int len;
				int retval;
				retval = recv(clientSock, (char*)&len, sizeof(int), 0);
				if (CheckSocketAndCloseOnError(retval, i)) continue;

				common::EPacketType type;
				retval = recv(clientSock, (char*)&type, sizeof(int), 0);
				if (CheckSocketAndCloseOnError(retval, i)) continue;

				std::string buffer;
				buffer.resize(len);
				retval = recv(clientSock, buffer.data(), len, 0);
				if (CheckSocketAndCloseOnError(retval, i)) continue;

				spdlog::info("[ChatServer] recv() data:{}", buffer);
				std::unique_lock<std::mutex> lock(WorkMutex);
				WorkList.push({ type, std::move(buffer), len });
				WorkCV.notify_one();
			}
		}
	}
}

void server::ChatServer::ProcessWorkList()
{
	while (!bIsEnd)
	{
		std::unique_lock<std::mutex> lock(WorkMutex);

		WorkCV.wait(lock, [this]()
			{
				return !WorkList.empty() || bIsEnd;
			});

		if (bIsEnd) break;

		while (!WorkList.empty())
		{
			common::Buffer work = std::move(WorkList.front());
			WorkList.pop();

			lock.unlock();

			// 작업처리 
			switch (work.Type)
			{
				case common::EPacketType::Message:
				{
					spdlog::info("Message Work Done");

					int retval;

					common::PMessage msg;
					msg.SetText(work.Data);
					std::string buffer;
					msg.WriteData(buffer);

					// 연결된 클라이언트에 메세지 보내기
					for (int i = 0; i < ClientSockets.size(); ++i)
					{
						retval = send(ClientSockets[i], buffer.data(), buffer.size(), 0);
						if (CheckSocketAndCloseOnError(retval, i)) continue;
					}
					spdlog::info("[ChatServer] send Text \"{}\"", msg.GetText());
				}
				break;
				case common::EPacketType::None:
				{
				}
				break;
				default:
					break;
			}

			lock.lock();
		}
	}
}

bool server::ChatServer::CheckSocketAndCloseOnError(int Retval, int index)
{
	if (common::Utill::CheckSocketError(Retval))
	{
		std::unique_lock lock(SocketMutex);
		closesocket(ClientSockets[index]);
		ClientSockets.erase(ClientSockets.begin() + index);
		return true;
	}

	return false;
}
