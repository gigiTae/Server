#include "Utill.h"


bool common::Utill::CheckSocketError(int Retval)
{
	if (Retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == WSAECONNRESET)
		{
			spdlog::debug("The connection was reset");
		}
		else
			spdlog::warn("SOCKET_ERROR [{}]", error);
		
		return true;
	}

	return false;
}
