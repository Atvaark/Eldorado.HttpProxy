#pragma once
#include <fstream>
#include <mutex>

class Logger
{
public:
	Logger(const wchar_t* log_file);
	~Logger();

	void Log(std::wstring message);
	void LogFormatA(const char* format, ...);
	void LogFormatW(const wchar_t* format, ...);
	void LogRawData(std::wstring filename, const char* data, const int dataLength);
private:
	std::wofstream log_stream;
	std::mutex log_mutex;
};

