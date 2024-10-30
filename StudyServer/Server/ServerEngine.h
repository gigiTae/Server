#pragma once

#include "../Common/BaseEngine.h"

namespace server
{
	class ServerEngine : public common::BaseEngine
	{
	public:
		ServerEngine() = default;
		~ServerEngine() = default;

	private:
		void StartProcess() override;
		void Update() override;
		void EndProcess() override;
	};

}