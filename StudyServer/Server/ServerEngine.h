#pragma once

namespace common
{
	class LogWindow;
	class WindowEngine;
}

namespace server
{
	class ChatServer;

	class ServerEngine 
	{
	public:
		ServerEngine() = default;
		~ServerEngine() = default;

		void Initialize();
		void Process();
		void Finalize();

	private:
		common::WindowEngine* mWindowEngine = nullptr;
		common::LogWindow* mLogWindow = nullptr;
		ChatServer* mChatServer = nullptr;
	};

}