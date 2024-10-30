#include "TestWindow.h"

#include <spdlog/spdlog.h>

void client::TestWindow::Update()
{

}

void client::TestWindow::RenderGUI()
{

}

void client::TestWindow::OnRegister()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
	}

	// socket()
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket == INVALID_SOCKET)
	{
		spdlog::error("socket()");
	}

	connect_scoket(900, "121.165.95.167");
}

void client::TestWindow::OnDestroy()
{

}

void client::TestWindow::connect_scoket(SHORT port, const char* ip)
{
	SOCKADDR_IN serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = inet_addr(ip);

	int check = connect(mSocket,(SOCKADDR*)&serverAddress, sizeof(serverAddress));

	if (check == SOCKET_ERROR)
	{
		spdlog::warn("connect_socket");
	}
}

