#pragma once

#include "../Common/BaseEngine.h"

namespace common
{
	class LogWindow;
}

namespace client
{
	class ClientEngine : public common::BaseEngine
	{
	public:
		ClientEngine() = default;
		~ClientEngine() = default;

	private:
		void StartProcess() override;
		void Update() override;
		void EndProcess() override;

	private:
		common::LogWindow* mLogWindow = nullptr;
	};


}