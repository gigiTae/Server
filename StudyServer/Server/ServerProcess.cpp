#include "ServerProcess.h"

#include "../Common/WindowManager.h"
#include "../Common/LogWindow.h"
#include "TCPNetwork.h"

void server::ServerProcess::Initialize()
{
	WindowMgr = new common::WindowManager();
	WindowMgr->Initialize(1000, 500, L"Server");

	LogMgr = new common::LogWindow();
	LogMgr->Initialize("Server");

	NetworkMgr = new TCPNetwork();
	ServerConfig config{};
	config.MaxClientCount = 3;
	config.ExtraClientCount = 1;
	config.Port = 9000;

	NetworkMgr->Initialize(config);
}

void server::ServerProcess::Process()
{
	while (WindowMgr->BeginLoof())
	{
		LogMgr->RenderGUI();
		WindowMgr->EndLoof();
	}
}

void server::ServerProcess::Finalize()
{
	WindowMgr->Finalize();
}
