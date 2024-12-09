#pragma once

#include "IClientNetwork.h"


namespace client
{
	class TCPClientNetwork : public IClientNetwork
	{
	public:
		TCPClientNetwork();
		virtual ~TCPClientNetwork();

		EClientErrorCode Initialize() override;
		void Finalize() override;

		EClientErrorCode Connect(short Port, const char* ServerIP) override;
		EClientErrorCode Disconnect() override;
		bool IsConnected() const override;

	protected:
		EClientErrorCode InitializeSocket();
		EClientErrorCode ConnectSocket(short Port, const char* ServerIP);
		EClientErrorCode CloseSocket();

	protected:
		SOCKET ClientSocket;
	};


}