#pragma once

namespace common
{
	enum class EPacketType
	{
		Message, // 메세지
	};

	/// <summary>
	/// 패킷 인터페이스 
	/// </summary>
	class IPacket
	{
	public:
		virtual ~IPacket(){}

		virtual int GetSize() abstract;
		virtual const char* GetData() abstract;
		virtual EPacketType GetType() abstract;
	};
}