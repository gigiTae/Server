#pragma once


namespace server
{
	enum class ENetErrorCode : short
	{
		None = 0,

		ServerSocketCreateFail = 101,
		ServerSocketsoReuseaddrFail = 102,
		ServerSocketBindFail = 103,
		ServerSocketListenFail = 104,
		ServerSocketFionbioFail = 105,
		ServerSocketAcceptFail = 106,
	};

}