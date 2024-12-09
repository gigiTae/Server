//#include "ClientManager.h"
//
//#include "../Common/Utill.h"
//
//void server::ClientManager::ConnectClient(ClientInfo Info)
//{
//	std::unique_lock lock(Mutex);
//	ClientList.insert({ Info.IPAddress ,Info });
//}
//
//void server::ClientManager::DisconnectClient(const std::string& IPAddress)
//{
//}
//
//server::ClientInfo server::ClientManager::GetClientInfo(const std::string& IPAddress) const
//{
//	std::shared_lock lock(Mutex);
//
//	if (ClientList.find(IPAddress) == ClientList.end())
//	{
//		return {};
//	}
//
//	return ClientInfo();
//}
//
//server::ClientManager::ClientMap server::ClientManager::GetClientList() const
//{
//	std::shared_lock lock(Mutex);
//	return ClientList;
//}
//
//void server::ClientManager::DeleteClient(const std::string& IPAddress)
//{
//	std::unique_lock lock(Mutex);
//
//	ClientList.erase(std::remove_if(ClientList.begin(), ClientList.end(),
//		[IPAddress](const ClientInfo& Info)
//		{
//			return Info.IPAddress == IPAddress;
//		}), ClientList.end());
//}
//
//void server::ClientManager::BroadCast(std::string& buffer)
//{
//	int retval;
//
//	for (auto iter = ClientList.begin(); iter != ClientList.end(); ++iter)
//	{
//		SOCKET sock = iter->second.Socket;
//		retval = send(sock, buffer.data(), buffer.size(), 0);
//		CheckSocketAndCloseOnError(retval, iter->second);
//	}
//}
//
//void server::ClientManager::CheckSocketAndCloseOnError(int Retavl, ClientInfo& Info)
//{
//	if (common::Utill::CheckSocketError(Retavl))
//	{
//		Info.bIsConnected = false;
//		closesocket(Info.Socket);
//		Info.Socket = INVALID_SOCKET;
//	}
//}
//
