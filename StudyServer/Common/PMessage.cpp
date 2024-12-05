#include "PMessage.h"

int common::PMessage::GetSize() const
{
	return sizeof(DataSize) + sizeof(EPacketType) + Text.size();
}

void common::PMessage::WriteData(std::string& buffer) const
{
	int dataSize = Text.size();
	EPacketType type = EPacketType::Message;

	buffer.resize(GetSize());
	memcpy(buffer.data(), &dataSize, sizeof(DataSize));
	memcpy(buffer.data() + sizeof(DataSize), &type, sizeof(EPacketType));
	memcpy(buffer.data() + sizeof(DataSize) + sizeof(EPacketType), Text.data(), Text.size());
}

