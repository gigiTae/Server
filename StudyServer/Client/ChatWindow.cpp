#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ChatWindow.h"

#include <spdlog/spdlog.h>
#include <future>
#include "../Common/ThreadPool.h"

bool client::ChatWindow::Initalize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		spdlog::critical("WSAStartup()");
		return false;
	}

	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		spdlog::critical("socket()");
		return false;
	}

	return true;
}

void client::ChatWindow::Finalize()
{


}

void client::ChatWindow::ProcessSend()
{
}


bool client::ChatWindow::ConnectSocket()
{
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = inet_addr(ServerIP);

	int retval = connect(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));

	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("connect() fail");
		return false;
	}

	spdlog::info("connect() success");
	
	std::unique_lock<std::mutex> lock(Mutex);
	State = EState::Connect;
	return true;
}

void client::ChatWindow::RenderGUI()
{
	std::unique_lock<std::mutex> lock(Mutex);

	if (ImGui::Begin("Chat"))
	{
		switch (State)
		{
		case client::ChatWindow::EState::Wait:
		{
			ImGui::InputText("IP", ServerIP, MAX_IP_LEN);
			if (ImGui::Button("Connect"))
			{
				common::ThreadPool::Get()->EnqueueJob([this]()
					{
						ConnectSocket();
					});
			}
		}
		break;
		case client::ChatWindow::EState::Connect:
		{
			// 메세지 창 
			ImGui::Text("TT");
			// 메세지 보내기 버튼
			static char buff[255];
			ImGui::InputText("Send", buff, 255);
		}
		break;
		}
	}
	ImGui::End();
}
