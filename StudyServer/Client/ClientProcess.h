#pragma once


namespace common
{
	class LogWindow;
	class WindowManager;
}

namespace client
{
	class ClientProcess 
	{
	public:
		ClientProcess() = default;
		~ClientProcess() = default;

	public:
		void Initialize();
		void Process();
		void Finalize();

	private:
		common::WindowManager* WindowMgr = nullptr;
		common::LogWindow* LogMgr = nullptr;
		class IClientNetwork* NetworkMgr = nullptr;
		class Login* LoginWindow = nullptr;
 	};
}