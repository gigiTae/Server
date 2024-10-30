#include "ClientEngine.h"

#include "../Common/LogWindow.h"
#include "TestWindow.h"

void client::ClientEngine::StartProcess()
{
	mLogWindow = RegisterWindow<common::LogWindow>();
	RegisterWindow<TestWindow>();
	
}

void client::ClientEngine::Update()
{
}

void client::ClientEngine::EndProcess()
{

}
