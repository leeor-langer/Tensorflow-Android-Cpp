#pragma once

#include <string>
#include <sstream>
#include <memory>

using namespace std;

#ifndef _WINDOWS
#include <android/log.h>
#endif

namespace Mudra
{
	namespace Computation
	{
		class Logger
		{
		public:
			enum class Severity
			{
				ExtraDebug,
				Debug,
				Info,
				Warning,
				Error,
				Fatal
			};

			Logger(const string &tag, Severity severityThreshold) : m_tag(tag), m_severityThreshold(severityThreshold) {};

			friend class LogMessage;

			void SetSeverityThreshold(Severity severity) { m_severityThreshold = severity; }
			Severity GetSeverityThreshold() const { return m_severityThreshold; }

			void ExtraDebug(const string &str) { Msg(Severity::ExtraDebug, str); }
			void Debug(const string &str) { Msg(Severity::Debug, str); }
			void Info(const string &str) { Msg(Severity::Info, str); }
			void Warning(const string &str) { Msg(Severity::Warning, str); }
			void Error(const string &str) { Msg(Severity::Error, str); }
			void Fatal(const string &str) { Msg(Severity::Fatal, str); }

		private:
			string m_tag;

			void Msg(Severity severity, const string &str);

			Severity m_severityThreshold;
		};


		class LogMessage : public basic_ostringstream<char>
		{
		public:
			LogMessage(Logger::Severity severity, shared_ptr<Logger> logger);
			~LogMessage();

		private:
			Logger::Severity m_severity;
			shared_ptr<Logger> m_logger;
		};

		class ExtraDebugMessage : public LogMessage
		{
		public:
			ExtraDebugMessage(shared_ptr<Logger> logger) : LogMessage(Logger::Severity::ExtraDebug, logger) {};
		};

		class DebugMessage : public LogMessage
		{
		public:
			DebugMessage(shared_ptr<Logger> logger) : LogMessage(Logger::Severity::Debug, logger) {};
		};

		class InfoMessage : public LogMessage
		{
		public:
			InfoMessage(shared_ptr<Logger> logger) : LogMessage(Logger::Severity::Info, logger) {};
		};

		class WarningMessage : public LogMessage
		{
		public:
			WarningMessage(shared_ptr<Logger> logger) : LogMessage(Logger::Severity::Warning, logger) {};
		};


		class ErrorMessage : public LogMessage
		{
		public:
			ErrorMessage(shared_ptr<Logger> logger) : LogMessage(Logger::Severity::Error, logger) {};
		};

		class FatalMessage : public LogMessage
		{
		public:
			FatalMessage(shared_ptr<Logger> logger) : LogMessage(Logger::Severity::Fatal, logger) {};
		};





		typedef LogMessage Log;
	}
}