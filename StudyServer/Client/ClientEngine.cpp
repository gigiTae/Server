#include "ClientEngine.h"

#include "../Common/LogWindow.h"

void client::ClientEngine::StartProcess()
{
	mLogWindow = RegisterWindow<common::LogWindow>();
}

void client::ClientEngine::Update()
{
}

void client::ClientEngine::EndProcess()
{

}
