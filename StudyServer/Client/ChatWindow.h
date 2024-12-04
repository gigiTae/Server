#pragma once

#include "../Common/IWindow.h"
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <mutex>
#pragma comment(lib, "Ws2_32.lib")

namespace client
{
	class ChatWindow : public common::IWindow
	{
		enum class EState
		{
			Wait,
			Connect,
		};

	public:
		ChatWindow() = default;
		~ChatWindow() = default;

		void RenderGUI() override;
		bool Initalize();
		void Finalize();

		void ProcessSend();

	private:
		bool ConnectSocket();

	private:
		static constexpr size_t MAX_IP_LEN = 16;

		char ServerIP[MAX_IP_LEN];

		EState State = EState::Wait;
		bool bIsConnect = false;
		SOCKET Socket;

		std::mutex Mutex;
	};
}

