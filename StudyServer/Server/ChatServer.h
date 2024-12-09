//#pragma once
//
//#include <ws2tcpip.h> 
//#include <winsock2.h>
//#include <vector>
//#include <mutex>
//#include <string>
//#include <queue>
//#include <memory>
//#pragma comment(lib, "Ws2_32.lib")
//
//namespace server
//{
//	class ChatServer
//	{
//	public:
//		ChatServer() = default;
//		~ChatServer() = default;
//
//		bool Initialize();
//		void Finalize();
//		
//		/// <summary>
//		/// 클라이언트 연결을 처리
//		/// </summary>
//		void ProcessClientPacket();
//		
//		void ProcessWorkList();
//
//	private:
//		bool CheckSocketAndCloseOnError(int Retval, int index);
//		bool InitializeServer();
//
//	private:
//		std::unique_ptr<class ClientManager> ClientMgr;
//		bool bIsEnd;
//
//		SOCKET ServerSocket;
//		SOCKADDR_IN ServerAddress;
//
//		std::queue<common::Buffer> WorkList;
//		std::mutex WorkMutex;
//		std::condition_variable WorkCV;
//	};
//}