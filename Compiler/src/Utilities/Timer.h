#pragma once

#include <chrono>


class Timer
{
public:
	Timer()
	{
		Reset();
	}

	void Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	float Elapsed()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
	}

	float ElapsedMillis()
	{
		return Elapsed() * 1000.0f;
	}

	std::string ElapsedFormat()
	{
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_Start).count();
		int minutes = elapsed / 60000;
		int seconds = (elapsed % 60000) / 1000;
		int milliseconds = elapsed % 1000;

		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(2) << minutes << ":"
			<< std::setfill('0') << std::setw(2) << seconds << "."
			<< std::setfill('0') << std::setw(3) << milliseconds;

		return oss.str();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

