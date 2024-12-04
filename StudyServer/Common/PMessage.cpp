#include "PMessage.h"

int common::PMessage::GetSize()
{
	return Text.size();
}

const char* common::PMessage::GetData()
{
	return Text.data();
}
