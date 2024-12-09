#pragma once

namespace server
{
	struct ServerConfig
	{
		unsigned short Port;
		int BackLogCount;

		int MaxClientCount;
		int ExtraClientCount;

		short MaxClientSockOptRecvBufferSize;
		short MaxClientSockOptSendBufferSize;
		short MaxClientRecvBufferSize;
		short MaxClientSendBufferSize;
	};

	struct PacketInfo
	{
		int SessionIndex;
		short PacketID;
		short Size;
		const char* Buffer;
	};

	constexpr int MAX_IP_LEN = 32;

	struct ClientSession
	{
		int Index = 0;
		long long Seq = 0;
		unsigned long long SocketFD = 0;
		char IP[MAX_IP_LEN] = { 0, };

		char* RecvBuffer = nullptr;
		int RemainingDataSize = 0;
		int PrevReadPosInRecvBuffer = 0;

		char* SendBuffer = nullptr;
		int SendSize = 0;
	};

	struct RecvPacketInfo
	{
		int SessionIndex = 0;
		short PacketID = 0;
		short PacketBodySize = 0;
		char* Data = nullptr;
	};

	enum class ESocketCloseCase : short
	{
		SessionPoolEmpty = 1,
	};

}