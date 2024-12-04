#pragma once

namespace common
{
	enum class EPacketType
	{
		Message, // �޼���
	};

	/// <summary>
	/// ��Ŷ �������̽� 
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