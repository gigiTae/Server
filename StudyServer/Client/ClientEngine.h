#pragma once

#include "../Common/BaseEngine.h"

namespace client
{
	class ClientEngine : public common::BaseEngine
	{
	public:
		ClientEngine() = default;
		~ClientEngine() = default;

	private:
		void Initialize() override;
		void Finalize() override;
		void RenderGUI() override;
		void Update() override;
	};


}