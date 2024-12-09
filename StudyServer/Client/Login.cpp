#include "Login.h"

#include "IClientNetwork.h"

client::Login::Login()
{
}

client::Login::~Login()
{
}

void client::Login::Initialize(IClientNetwork* InNetworkMgr)
{
	NetworkMgr = InNetworkMgr;
}

void client::Login::RenderGUI()
{
	if (ImGui::Begin("LogWinodw"))
	{
		ImGui::InputText("Server IP", ServerIP, MAX_IP_LEN);

		if (NetworkMgr->IsConnected())
		{
			if (ImGui::Button("Disconnect"))
			{
				auto error = NetworkMgr->Disconnect();
				if (error != EClientErrorCode::None) spdlog::warn("DisconnectServer Fail");
			}
		}
		else
		{
			if (ImGui::Button("Connect"))
			{
				auto error = NetworkMgr->Connect(9000, ServerIP);
				if (error != EClientErrorCode::None) spdlog::warn("ConnectServer Fail");
			}
		}

	}
	ImGui::End();
}

