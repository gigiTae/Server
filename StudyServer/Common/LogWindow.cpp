#include "LogWindow.h"

#include <ctime> 
#include <string>
#include <iomanip>

#include "ImGuiColor.h"


void common::LogWindow::RenderGUI()
{
	if (ImGui::Begin("Log"))
	{
		for (const auto& log : mLogList)
		{
			renderLog(log);
		}
	}
	ImGui::End();
}

void common::LogWindow::Initialize(const char* name)
{
	auto callbackSink = std::make_shared<spdlog::sinks::callback_sink_mt>(
		[this](const spdlog::details::log_msg& msg)
		{
			this->AddLog(msg);
		});

	spdlog::logger logger(name, { callbackSink });
	logger.set_level(spdlog::level::trace);

	spdlog::set_default_logger(logger.clone(name));
	mLogger = spdlog::get(name);
}


void common::LogWindow::renderLog(const LogMessage& msg)
{
	// Time 
	ImGui::Text(msg.time.c_str());
	ImGui::SameLine();

	// level
	std::string level{};
	if ("[trace]" == msg.level)
	{
		ImGui::PushStyleColor(0, ImGuiColor::STEEL_GRAY);
	}
	else if ("[dubug]" == msg.level)
	{
		ImGui::PushStyleColor(0, ImGuiColor::TURQUOISE);
	}
	else if ("[info]" == msg.level)
	{
		ImGui::PushStyleColor(0, ImGuiColor::SPRING_GREEN);
	}
	else if ("[warn]" == msg.level)
	{
		ImGui::PushStyleColor(0, ImGuiColor::ORANGE);
	}
	else if ("[error]" == msg.level)
	{
		ImGui::PushStyleColor(0, ImGuiColor::YELLOW);
	}
	else if ("[critical]" == msg.level)
	{
		ImGui::PushStyleColor(0, ImGuiColor::RED);
	}

	ImGui::Text(msg.level.c_str());
	ImGui::PopStyleColor();
	ImGui::SameLine();
	std::string loggerName = "[" + msg.loggerName + "]";
	ImGui::Text(loggerName.c_str());

	// payload
	ImGui::SameLine();
	ImGui::Text(msg.payload.c_str());

	ImGui::Separator();
}

void common::LogWindow::AddLog(spdlog::details::log_msg msg)
{
	if (MaxSize < mLogList.size())
	{
		mLogList.pop_back();
	}

	std::time_t time = std::chrono::system_clock::to_time_t(msg.time);
	std::tm now_tm;
	localtime_s(&now_tm, &time);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &now_tm);

	LogMessage logMsg;
	logMsg.time = buffer;

	if (spdlog::level::trace == msg.level)		   logMsg.level = "[trace]";
	else if (spdlog::level::debug == msg.level)    logMsg.level = "[dubug]";
	else if (spdlog::level::info == msg.level)	   logMsg.level = "[info]";
	else if (spdlog::level::warn == msg.level)	   logMsg.level = "[warn]";
	else if (spdlog::level::err == msg.level)	   logMsg.level = "[error]";
	else if (spdlog::level::critical == msg.level) logMsg.level = "[critical]";

	logMsg.loggerName = msg.logger_name.data();
	logMsg.payload = msg.payload.data();
	mLogList.push_front(logMsg);
}

