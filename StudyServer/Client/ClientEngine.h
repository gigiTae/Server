#pragma once


namespace common
{
	class LogWindow;
	class WindowManager;
}

namespace client
{
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
		common::WindowManager* mWindowEngine = nullptr;
		common::LogWindow* mLogWindow = nullptr;
	};
}