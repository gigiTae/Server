#pragma once
#include "IWindow.h"

#include <list>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/callback_sink.h>

namespace common
{
	struct LogMessage
	{
		std::string time;
		std::string loggerName;
		std::string payload;
		std::string level;
	};

	class LogWindow : public IWindow
	{
	public:
		LogWindow() = default;
		~LogWindow() = default;

	public:
		void Initialize(const char* name);
		void RenderGUI() override;
		void AddLog(spdlog::details::log_msg msg);

	private:
		void renderLog(const LogMessage& msg);

	private:
		static const UINT MaxSize = 100;

		std::shared_ptr<spdlog::logger> mLogger;
		std::list<LogMessage> mLogList;
	};


}