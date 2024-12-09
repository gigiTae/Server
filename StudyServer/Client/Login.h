#pragma once

#include "../Common/IWindow.h"
#include "Define.h"

namespace client
{
	class IClientNetwork;

	class Login : public common::IWindow
	{
	public:
		Login();
		~Login();

		void Initialize(IClientNetwork* InNetworkMgr);

		void RenderGUI() override;

	private:
		IClientNetwork* NetworkMgr = nullptr;
		
		char ServerIP[MAX_IP_LEN];
	};
}