#pragma once

#include "../Common/IWindow.h"
#include <winsock2.h>
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib")

namespace client
{
	class ChatWindow : public common::IWindow
	{
		enum class State
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

	private:
		bool connectSocket();

	private:
		State mState = State::Wait;
		bool mIsConnect = false;
		SOCKET mSocket;
	};
}

