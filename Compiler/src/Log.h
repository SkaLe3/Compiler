#ifndef LOG_H_
#define LOG_H_

#include <iostream>

class Log
{
public:
	static void Init();

	static Log* GetLogger() { return s_Logger; }
	static void SetSeparator(char separator) { GetLogger()->m_Separator = separator; }

	template <typename... Types>
	void State(Types... objects)
	{
		SetColor(1, 3);
		PrintTime();
		Print(objects...);
		SetColor(0, 0);
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
		SetColor(1, 1);
		PrintTime();
		Print(objects...);
		SetColor(0, 0);
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
	void SetColor(int brightness, int color);
	void PrintTime();

private:
	static Log* s_Logger;
	char m_Separator;
};

#ifdef COMPILE_DEBUG
#define LOG_STATE(...) Log::GetLogger()->State(__VA_ARGS__);
#define LOG_ERROR(...) Log::GetLogger()->Error(__VA_ARGS__);
#define LOG_TRACE(...) Log::GetLogger()->Trace(__VA_ARGS__);

#else
#define LOG_STATE
#define LOG_ERROR
#define LOG_TRACE
#endif


#endif /* LOG_H_ */