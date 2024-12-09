#include "ClientEngine.h"

#include <spdlog/spdlog.h>

#include "../Common/WindowManager.h"
#include "../Common/LogWindow.h"


void client::ClientEngine::Initialize()
{
	mWindowEngine = new common::WindowManager();
	mWindowEngine->Initialize(1000, 500, L"Client", 0, 500);

	mLogWindow = new common::LogWindow();
	mLogWindow->Initialize("Client");

}

void client::ClientEngine::Process()
{
	while (mWindowEngine->BeginLoof())
	{
		mLogWindow->RenderGUI();
		mWindowEngine->EndLoof();
	}
}

void client::ClientEngine::Finalize()
{
	mWindowEngine->Finalize();
}
