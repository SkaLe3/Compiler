#include "Log.h"
#include <Windows.h>
#include <ctime>
#include <string>

Log* Log::s_Logger;

void Log::Init()
{
	s_Logger = new Log;
	s_Logger->m_Separator = ' ';
	LOG_STATE("Initialized Log!");
}

void Log::SetColor(int brightness,  int color)
{	
	if (color == 0)
	{
		std::cout << "\033[0m";
		return;
	}
	std::string st = std::string("\033[") + std::to_string(brightness) + ";3" + std::to_string(color) + "m";
	std::cout << st;
}

void Log::PrintTime()
{
	time_t now = time(0);
	tm* lctime = localtime(&now);
	auto h = lctime->tm_hour;
	auto m = lctime->tm_min;
	auto s = lctime->tm_sec;

	std::cout << '[' << (h > 9 ? "\0" : "0") << h << ':' << (m > 9 ? "\0" : "0") << m << ':' << (s > 9 ? "\0" : "0") << s << "] ";
}