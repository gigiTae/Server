#pragma once

namespace common
{
	class LogWindow;
	class WindowManager;
}

namespace server
{
	class ServerProcess 
	{
	public:
		ServerProcess() = default;
		~ServerProcess() = default;

		void Initialize();
		void Process();
		void Finalize();

	protected:

	private:
		common::WindowManager* WindowMgr = nullptr;
		common::LogWindow* LogMgr = nullptr;
		class INetWork* NetworkMgr = nullptr;
	};

}