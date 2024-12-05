#pragma once

#include <string>

namespace common
{
	enum class EPacketType : int 
	{
		Message = 0, // 메세지
		None,
	};

	/// <summary>
	/// 패킷 인터페이스 
	/// </summary>
	class IPacket
	{
	public:
		using DataSize = int;

		virtual ~IPacket(){}

		virtual int GetSize() const abstract;
		virtual void WriteData(std::string& buffer) const abstract;
		virtual EPacketType GetType() const abstract;
	};
}