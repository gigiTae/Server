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
		void Initialize() override;
		void Finalize() override;
		void RenderGUI() override;
		void Update() override;
	};

}