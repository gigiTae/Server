//#pragma once
//
//#include <unordered_map>
//#include <shared_mutex>
//#include "ClientInfo.h"
//
//namespace server
//{
//	class ClientManager
//	{
//	public:
////		using ClientMap = std::unordered_map<std::string, ClientInfo>;
//
//		ClientManager() = default;
//		~ClientManager() = default;
//
//		void ConnectClient(ClientInfo Info);
//		void DisconnectClient(const std::string& IPAddress);
//		
//		ClientInfo GetClientInfo(const std::string& IPAddress)const;
//
//		ClientMap GetClientList()const;
//		void DeleteClient(const std::string& IPAddress);
//		void BroadCast(std::string& buffer);
//
//	private:
//		void CheckSocketAndCloseOnError(int Retavl , ClientInfo& Info);
//
//	private:
//		ClientMap ClientList;
//		mutable std::shared_mutex Mutex;
//	};
//}
//
