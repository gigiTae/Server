#pragma once

#include "IPacket.h"

namespace common
{
	struct Buffer
	{
		EPacketType Type = EPacketType::None;
		std::string Data{};
		int DataSize = 0;
	};
}
