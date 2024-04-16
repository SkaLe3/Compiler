#pragma once

#include <iostream>

#define CRIMSON   "\033[31m"
#define LIME	  "\033[32m"
#define LEMON	  "\033[33m"
#define AZURE	  "\033[34m"
#define MAGENTA   "\033[35m"
#define TEAL	  "\033[36m"

#define RESET     "\033[0m"
#define UNDERLINE "\033[4m"
#define INVERSE   "\033[7m"

class Log
{
public:
	static void Init();

	static Log* GetLogger() { return s_Logger; }
	static void SetSeparator(char separator) { GetLogger()->m_Separator = separator; }

	template <typename... Types>
	void State(Types... objects)
	{
		SetColor(LEMON);
		PrintTime();
		Print(objects...);
		SetColor(RESET);
	}

	template <typename... Types>
	void Trace(Types... objects)
	{
		PrintTime();
		Print(objects...);
	}

	template <typename... Types>
	void Error(Types... objects)
	{
		SetColor(CRIMSON);
		PrintTime();
		Print(objects...);
		SetColor(RESET);
	}
private:
	template <typename T>
	void Print(const T& obj)
	{
		std::cout << obj << "\n";
	}
	template <typename T, typename... Types>
	void Print(const T& obj, const Types&... objects)
	{
		std::cout << obj << m_Separator;
		Print(objects...);
	}
	void SetColor(const char* color);
	void PrintTime();

private:
	static Log* s_Logger;
	char m_Separator;
};


#define LOG_STATE(...) Log::GetLogger()->State(__VA_ARGS__);
#define LOG_ERROR(...) Log::GetLogger()->Error(__VA_ARGS__);
#define LOG_TRACE(...) Log::GetLogger()->Trace(__VA_ARGS__);


