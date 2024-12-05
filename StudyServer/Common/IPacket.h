#pragma once

#include <string>

namespace common
{
	enum class EPacketType : int 
	{
		Message = 0, // �޼���
		None,
	};

	/// <summary>
	/// ��Ŷ �������̽� 
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