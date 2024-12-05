#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ChatWindow.h"

#include <spdlog/spdlog.h>
#include <future>
#include "../Common/ThreadPool.h"
#include "../Common/PMessage.h"
#include "../Common/Utill.h"

bool client::ChatWindow::Initalize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		spdlog::critical("WSAStartup()");
		return false;
	}

	common::ThreadPool::Get()->EnqueueJob([this]() { ProcessWorkList(); });
	return true;
}

void client::ChatWindow::Finalize()
{
	bIsEnd = true;
	closesocket(Socket);
	WSACleanup();
}

void client::ChatWindow::ProcessWorkList()
{
	while (!bIsEnd)
	{
		std::unique_lock<std::mutex> lock(WorkMutex);

		WorkCV.wait(lock, [this]()
			{
				return !WorkList.empty() || bIsEnd;
			});

		if (bIsEnd) break;

		while (!WorkList.empty())
		{
			common::Buffer work = std::move(WorkList.front());
			WorkList.pop();
			lock.unlock();

			// 작업처리 
			switch (work.Type)
			{
				case common::EPacketType::Message:
				{
					std::unique_lock lock(ChatDataMutex);
					ChatData.push_back(std::move(work.Data));
				}
				break;
				case common::EPacketType::None:
				break;
				default:
					break;
			}

			lock.lock();
		}
	}
}

void client::ChatWindow::ProcessServerPacket()
{
	while (!bIsEnd)
	{
		int len{};
		int retval{};
		retval = recv(Socket, (char*)&len, sizeof(int), 0);
		if (CheckSocketAndCloseOnError(retval)) return;

		common::EPacketType type;
		retval = recv(Socket, (char*)&type, sizeof(int), 0);
		if (CheckSocketAndCloseOnError(retval)) return;

		std::string buffer;
		buffer.resize(len);
		retval = recv(Socket, buffer.data(), len, 0);
		if (CheckSocketAndCloseOnError(retval)) return;

		spdlog::info("[ChatWindow] recv() len:{} type:{} data:{}", len, (int)type, buffer);

		std::unique_lock<std::mutex> lock(WorkMutex);
		WorkList.push({ type, std::move(buffer), len });
		WorkCV.notify_one();
	}
}

bool client::ChatWindow::ConnectSocket()
{
	if (Socket == INVALID_SOCKET)
	{
		Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (Socket == INVALID_SOCKET)
		{
			spdlog::critical("socket()");
			return false;
		}
	}

	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = inet_addr(ServerIP);

	int retval = connect(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));

	if (retval == SOCKET_ERROR)
	{
		spdlog::warn("connect() fail");
		return false;
	}

	spdlog::info("connect() success");

	State = EState::Connect;
	bIsConnect = true;

	common::ThreadPool::Get()->EnqueueJob([this]() { ProcessServerPacket(); });
	return true;
}

void client::ChatWindow::SendText()
{
	common::PMessage msg;
	msg.SetText(TextBuffer);

	ZeroMemory(TextBuffer, MAX_TEXT_LEN);

	int retval;
	std::string buffer;
	msg.WriteData(buffer);

	retval = send(Socket, buffer.data(), buffer.size(), 0);
	if (CheckSocketAndCloseOnError(retval)) return;

	spdlog::info("[ChatWindow] Send Text \"{}\"", msg.GetText());
}

bool client::ChatWindow::CheckSocketAndCloseOnError(int retval)
{
	if (common::Utill::CheckSocketError(retval))
	{
		closesocket(Socket);
		Socket = INVALID_SOCKET;
		State = EState::Wait;
		bIsConnect = false;

		// 기존의 채팅 삭제 
		std::unique_lock lock(ChatDataMutex);
		ChatData.clear();

		return true;
	}

	return false;
}

void client::ChatWindow::RenderGUI()
{
	if (ImGui::Begin("Chat"))
	{
		switch (State)
		{
			case client::ChatWindow::EState::Wait:
			{
				ImGui::InputText("IP", ServerIP, MAX_IP_LEN);
				if (ImGui::Button("Connect"))
				{
					common::ThreadPool::Get()->EnqueueJob([this]()
						{
							ConnectSocket();
						});
				}
			}
			break;
			case client::ChatWindow::EState::Connect:
			{
				// 메세지 창 
				std::unique_lock lock(ChatDataMutex);

				for (const auto& chat : ChatData)
				{
					ImGui::Text(chat.c_str());
				}

				lock.unlock();

				ImGui::Separator();

				// 메세지 보내기
				ImGui::InputText("Send", TextBuffer, MAX_TEXT_LEN);
				if (ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					SendText();
				}
			}
			break;
		}
	}
	ImGui::End();
}
