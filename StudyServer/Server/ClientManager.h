#pragma once

#include <vector>
#include <shared_mutex>
#include "ClientInfo.h"

namespace server
{
	class ClientManager
	{
	public:
		ClientManager() = default;
		~ClientManager() = default;

		void AddClient(ClientInfo Info);
		std::vector<ClientInfo> GetClientInfos()const;
		void DeleteClient(const std::string& IPAddress);

		void BroadCast(std::string& buffer);

	private:
		std::vector<ClientInfo> ClientInfos;
		mutable std::shared_mutex Mutex;
	};
}

