#pragma once
#include "IPacket.h"

#include <string>

namespace common
{
	/// <summary>
	/// �޼��� ��Ŷ
	/// </summary>
	class PMessage : public IPacket
	{
	public: 
		const std::string& GetText()const { return Text; }
		void SetText(const std::string& InText) { Text = InText; }
		
		int GetSize() const override;
		void WriteData(std::string& buffer) const override;
		EPacketType GetType() const override { return EPacketType::Message; }
	private:
		std::string Text;
	};
}