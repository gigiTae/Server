#pragma once


namespace common
{
	class LogWindow;
	class WindowEngine;
}

namespace client
{
	class ChatWindow;

	class ClientEngine 
	{
	public:
		ClientEngine() = default;
		~ClientEngine() = default;

	public:
		void Initialize();
		void Process();
		void Finalize();

	private:
		common::WindowEngine* mWindowEngine = nullptr;
		common::LogWindow* mLogWindow = nullptr;
		ChatWindow* mChatWindow = nullptr;
	};


}