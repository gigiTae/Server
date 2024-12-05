#pragma once

#include <winsock2.h>
#include <ws2tcpip.h> 
#include <spdlog/spdlog.h>

namespace common
{
	class Utill
	{
	public:
		static bool CheckSocketError(int Retval);
	};
}