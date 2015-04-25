#include "Logger.h"
#include <stdarg.h>
#include <time.h>
#include <iomanip>

Logger::Logger(const _TCHAR* log_file)
{
	log_stream.open(log_file, std::ios::out | std::ios::app);
}

Logger::~Logger()
{
	log_stream.close();
}

void Logger::Log(std::wstring message)
{
	time_t time = std::time(nullptr);
	tm localtime;
	localtime_s(&localtime, &time);
	log_stream << _T("[") << std::put_time(&localtime, _T("%d.%m.%Y %H:%M:%S")) << _T("] ") << message.c_str() << _T("\n") << std::flush;
}

void Logger::LogFormat(const _TCHAR* format, ...)
{
	const int messageSize = 131072;
	_TCHAR message[messageSize];
	va_list args;
	va_start(args, format);
	_vstprintf_s(message, messageSize, format, args);
	time_t time = std::time(nullptr);
	tm localtime;
	localtime_s(&localtime, &time);
	log_stream << _T("[") << std::put_time(&localtime, _T("%d.%m.%Y %H:%M:%S")) << _T("] ") << message << _T("\n") << std::flush;
	va_end(args);
}

void Logger::LogFormatA(const char* format, ...)
{
	const int messageSize = 131072;
	char formatMessageBuffer[messageSize];
	wchar_t message[messageSize];
	va_list args;
	va_start(args, format);
	vsprintf_s(formatMessageBuffer, messageSize, format, args);
	size_t charsConverted;
	mbstowcs_s(&charsConverted, message, formatMessageBuffer, messageSize);
	Log(message);
	va_end(args);
}

void Logger::LogFormatW(const wchar_t* format, ...)
{
	const int messageSize = 131072;
	wchar_t message[messageSize];
	va_list args;
	va_start(args, format);
	vswprintf_s(message, messageSize, format, args);
	Log(message);
	va_end(args);
}