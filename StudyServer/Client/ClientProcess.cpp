#include "ClientProcess.h"

#include <spdlog/spdlog.h>

#include "../Common/WindowManager.h"
#include "../Common/LogWindow.h"
#include "TCPClientNetwork.h"
#include "Login.h"

void client::ClientProcess::Initialize()
{
	WindowMgr = new common::WindowManager();
	WindowMgr->Initialize(1000, 500, L"Client", 0, 500);

	LogMgr = new common::LogWindow();
	LogMgr->Initialize("Client");

	NetworkMgr = new TCPClientNetwork();
	NetworkMgr->Initialize();

	LoginWindow = new Login();
	LoginWindow->Initialize(NetworkMgr);
}

void client::ClientProcess::Process()
{
	while (WindowMgr->BeginLoof())
	{
		LogMgr->RenderGUI();
		LoginWindow->RenderGUI();
		WindowMgr->EndLoof();
	}
}

void client::ClientProcess::Finalize()
{
	WindowMgr->Finalize();
}
