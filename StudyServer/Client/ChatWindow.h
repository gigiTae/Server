//#pragma once
//
//#include "../Common/IWindow.h"
//#include <winsock2.h>
//#include <ws2tcpip.h> 
//#include <mutex>
//#include <queue>
//
//#include "../Common/Buffer.h"
//
//#pragma comment(lib, "Ws2_32.lib")
//
//namespace client
//{
//	class ChatWindow : public common::IWindow
//	{
//		enum class EState
//		{
//			Wait,
//			Connect,
//		};
//
//	public:
//		ChatWindow() = default;
//		~ChatWindow() = default;
//
//		void RenderGUI() override;
//		bool Initalize();
//		void Finalize();
//
//		void ProcessWorkList();
//		void ProcessServerPacket();
//
//	private:
//		bool ConnectSocket();
//		void SendText();
//		bool CheckSocketAndCloseOnError(int retval);
//
//	private:
//		static constexpr size_t MAX_IP_LEN = 16;
//		static constexpr size_t MAX_TEXT_LEN = 255;
//
//		bool bIsEnd;
//		bool bIsConnect = false;
//
//		char ServerIP[MAX_IP_LEN];
//		char TextBuffer[MAX_TEXT_LEN];
//		EState State = EState::Wait;
//		SOCKET Socket = INVALID_SOCKET;
//
//		std::queue<common::Buffer> WorkList;
//		std::mutex WorkMutex;
//		std::condition_variable WorkCV;
//
//		std::vector<std::string> ChatData;
//		std::mutex ChatDataMutex;
//	};
//}
//
