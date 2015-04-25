#pragma once
#include <tchar.h>
#include <utility>
#include <fstream>

class Logger
{
public:
	Logger(const _TCHAR* log_file);
	~Logger();

	void Log(std::wstring message);
	void LogFormat(const _TCHAR* format, ...);
	void LogFormatA(const char* format, ...);
	void LogFormatW(const wchar_t* format, ...);

private:
	std::wofstream log_stream;
};

