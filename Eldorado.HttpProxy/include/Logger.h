#pragma once
#include <utility>
#include <fstream>

class Logger
{
public:
	Logger(const wchar_t* log_file);
	~Logger();

	void Log(std::wstring message);
	void LogFormatA(const char* format, ...);
	void LogFormatW(const wchar_t* format, ...);

private:
	std::wofstream log_stream;
};

