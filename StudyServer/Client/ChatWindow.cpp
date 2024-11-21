#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ChatWindow.h"

#include <spdlog/spdlog.h>
#include <future>

bool client::ChatWindow::Initalize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		spdlog::critical("WSAStartup()");
		return false;
	}

	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket == INVALID_SOCKET)
	{
		spdlog::critical("socket()");
		return false;
	}


	return true;
}

void client::ChatWindow::Finalize()
{


}


bool client::ChatWindow::connectSocket()
{
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = inet_addr("121.169.139.84");
	int retval = connect(mSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));

	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("connect() fail");
		return false;
	}

	spdlog::trace("connect() success");

	mState = State::Connect;
	return true;
}

void client::ChatWindow::RenderGUI()
{
	if (ImGui::Begin("Chat"))
	{
		switch (mState)
		{
		case client::ChatWindow::State::Wait:

		{
			if (ImGui::Button("Connect"))
			{
			}
		}
		break;
		case client::ChatWindow::State::Connect:
		{
			ImGui::Text("Connect");
		}
		break;
		}
	}
	ImGui::End();
}
