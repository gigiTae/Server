#pragma once


namespace server
{
	enum class ENetErrorCode : short
	{
		None = 0,

		SERVER_SOCKET_CREATE_FAIL = 101,
		SERVER_SOCKET_SO_REUSEADDR_FAIL = 102,
		SERVER_SOCKET_BIND_FAIL = 103,
		SERVER_SOCKET_LISTEN_FAIL = 104,
		SERVER_SOCKET_FIONBIO_FAIL = 105,
		SERVER_SOCKET_ACCEPT_FAIL = 106,
	};

}