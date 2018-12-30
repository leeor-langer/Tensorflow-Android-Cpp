#include <iostream>

#include "Logging.h"

using namespace Mudra::Computation;
using namespace std;


LogMessage::LogMessage(Logger::Severity severity, shared_ptr<Logger> logger)
	: m_severity(severity), m_logger(logger)
{
}

LogMessage::~LogMessage()
{
	m_logger->Msg(m_severity, str());
}

void Logger::Msg(Severity severity, const string &str)
{
	if (m_severityThreshold > severity)
	{
		return;
	}

#ifdef _WINDOWS
	cout << str;
#else
	android_LogPriority androidSevirity;

	switch (severity)
	{
	case Logger::Severity::Debug:
	case Logger::Severity::ExtraDebug:
		androidSevirity = ANDROID_LOG_DEBUG;
		break;
	case Logger::Severity::Info:
		androidSevirity = ANDROID_LOG_INFO;
		break;
	case Logger::Severity::Warning:
		androidSevirity = ANDROID_LOG_WARN;
		break;
	case Logger::Severity::Error:
		androidSevirity = ANDROID_LOG_ERROR;
		break;
	case Logger::Severity::Fatal:
		androidSevirity = ANDROID_LOG_FATAL;
		break;
	default:
		androidSevirity = ANDROID_LOG_UNKNOWN;
		break;
	}

	__android_log_write(androidSevirity, m_tag.c_str(), str.c_str());
#endif

}
