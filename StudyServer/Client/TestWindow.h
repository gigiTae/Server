#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "../Common/IWindow.h"

#include <ws2tcpip.h> 
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

namespace client
{
	class TestWindow : public common::IWindow
	{
	public:
		TestWindow() = default;
		~TestWindow() = default;

	private:
		void Update() override;
		void RenderGUI() override;
		void OnRegister() override;
		void OnDestroy() override;

	private:
		void connect_scoket(SHORT port, const char* ip);

	private:
		SOCKET mSocket;


	};

}
