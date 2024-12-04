#pragma once
#include "IPacket.h"

#include <string>

namespace common
{
	/// <summary>
	/// 메세지 패킷
	/// </summary>
	class PMessage : public IPacket
	{
	public:
		const std::string& GetText()const { return Text; }
		void SetText(const std::string& InText) { Text = InText; }
		
		int GetSize() override;
		const char* GetData() override;
		EPacketType GetType() override { return EPacketType::Message; }
	private:
		std::string Text;
	};
}