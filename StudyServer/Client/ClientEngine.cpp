#include "ClientEngine.h"

#include <spdlog/spdlog.h>

#include "../Common/WindowEngine.h"
#include "../Common/LogWindow.h"

#include "ChatWindow.h"

void client::ClientEngine::Initialize()
{
	mWindowEngine = new common::WindowEngine();
	mWindowEngine->Initialize(1000, 500, L"Client");

	mLogWindow = new common::LogWindow();
	mLogWindow->Initialize("Client");

	mChatWindow = new ChatWindow();
	mChatWindow->Initalize();
}

void client::ClientEngine::Process()
{
	while (mWindowEngine->BeginLoof())
	{
		mChatWindow->RenderGUI();
		mLogWindow->RenderGUI();
		mWindowEngine->EndLoof();
	}
}

void client::ClientEngine::Finalize()
{
	mChatWindow->Finalize();
	mWindowEngine->Finalize();
}
