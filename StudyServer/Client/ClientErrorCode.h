#pragma once

#include <spdlog/spdlog.h>

namespace client
{
	enum class EClientErrorCode : short
	{
		None = 0,

		ClientSocketCreateFail = 101,
		ClientSocketConnectFail = 102,

	};

}