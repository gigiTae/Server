#pragma once

#include "PacketID.h"
#include "ErrorCode.h"

namespace common
{
#pragma pack(push, 1)
	struct PacketHeader
	{
		short TotalSize;
		short ID;
	};



#pragma pack(pop)
}