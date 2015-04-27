#include "Logger.h"
#include <fstream>
#include <iomanip>
#include <stdarg.h>
#include <time.h>
#include <Windows.h>

Logger::Logger(const wchar_t* log_file)
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
	log_stream << L"[" << std::put_time(&localtime, L"%d.%m.%Y %H:%M:%S") << L"] ";
	log_stream << message.c_str() << L"\n" << std::flush;
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

void Logger::LogRawData(std::wstring filePrefix, const char* data, const int dataLength)
{
	std::wstring filePath = L"winhttp";
	CreateDirectory(filePath.c_str(), nullptr);	
	wchar_t tempFilePath[MAX_PATH];
	GetTempFileName(filePath.c_str(), filePrefix.c_str(), 0, tempFilePath);

	LogFormatW(L"Logging raw data to %s", tempFilePath);

	std::ofstream outfile;
	outfile.open(tempFilePath, std::ios::binary | std::ios::out);
	if (!outfile.is_open())
	{
		LogFormatW(L"Logging raw data to %s failed. Could not create temporary file.", tempFilePath);
		return;
	}
	outfile.write(data, dataLength);
	LogFormatW(L"Logged %d bytes raw data to %s", dataLength, tempFilePath);
	outfile.close();
}