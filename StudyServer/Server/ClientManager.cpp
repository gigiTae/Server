#include "ClientManager.h"

void server::ClientManager::AddClient(ClientInfo Info)
{
	std::unique_lock lock(Mutex);
	ClientInfos.push_back(Info);
}

std::vector<server::ClientInfo> server::ClientManager::GetClientInfos() const
{
	std::shared_lock lock(Mutex);
	return ClientInfos;
}

void server::ClientManager::DeleteClient(const std::string& IPAddress)
{
	std::unique_lock lock(Mutex);

	ClientInfos.erase(std::remove_if(ClientInfos.begin(), ClientInfos.end(),
		[IPAddress](const ClientInfo& Info)
		{
			return Info.IPAddress == IPAddress;
		}), ClientInfos.end());
}

void server::ClientManager::BroadCast(std::string& buffer)
{
	int retval;

	for (int i = 0; i < ClientInfos.size(); ++i)
	{
		SOCKET sock = ClientInfos[i].Socket;

		retval = send(sock, buffer.data(), buffer.size(), 0);
	}
}
