#include "Log.h"

#include <ctime>
#include <string>

Log* Log::s_Logger;

void Log::Init()
{
	s_Logger = new Log;
	s_Logger->m_Separator = ' ';
	LOG_STATE("Initialized Log!");
}

void Log::SetColor(const char* color)
{	
	std::cout << color;
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