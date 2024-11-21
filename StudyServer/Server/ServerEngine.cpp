#include "ServerEngine.h"

#include "../Common/WindowEngine.h"
#include "../Common/LogWindow.h"
#include "ChatServer.h"

void server::ServerEngine::Initialize()
{
	mWindowEngine = new common::WindowEngine();
	mWindowEngine->Initialize(1000, 500, L"Server");

	mLogWindow = new common::LogWindow();
	mLogWindow->Initialize("Server");

	mChatServer = new ChatServer();
	mChatServer->Initialize();
}

void server::ServerEngine::Process()
{
	while (mWindowEngine->BeginLoof())
	{
		mLogWindow->RenderGUI();

		mWindowEngine->EndLoof();
	}
}

void server::ServerEngine::Finalize()
{
	mChatServer->Finalize();

	mWindowEngine->Finalize();
}
