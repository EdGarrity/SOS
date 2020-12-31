#pragma once

#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <time.h>
#include <ctime>

namespace Utilities
{
	extern std::mutex work_order_print_;
	extern unsigned long line_number;

	inline std::string getCurrentTimestamp()
	{
		using std::chrono::system_clock;
		auto currentTime = std::chrono::system_clock::now();
		char buffer[80];

		auto transformed = currentTime.time_since_epoch().count() / 1000000;

		//auto millis = transformed % 1000;
		unsigned long nano_seconds = transformed % 1000000000;

		std::time_t tt;
		tt = system_clock::to_time_t(currentTime);

		struct tm newtime;
		localtime_s(&newtime, &tt);

		strftime(buffer, 80, "%F %H:%M:%S", &newtime);
		sprintf_s(buffer, "%s.%06ld", buffer, nano_seconds);

		return std::string(buffer);
	}

	inline void debug_log(const int env_index, std::string function, std::string status)
	{
		static std::string prev_status = "";

		if (prev_status != status)
		{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

			std::cout << getCurrentTimestamp()
				<< ",LineNumber=" << std::to_string(line_number++)
				<< ",Thread=" << env_index
				<< ",Function=" << function
				<< ",Status=" << status
				<< std::endl;
		}
	}

	inline void debug_log(const int env_index, 
		std::string function, 
		std::string status, 
		unsigned int individual_index, 
		unsigned int example_case)
	{
		static std::string prev_status = "";

		if (prev_status != status)
		{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

			std::cout << getCurrentTimestamp()
				<< ",LineNumber=" << std::to_string(line_number++)
				<< ",Thread=" << env_index
				<< ",Function=" << function
				<< ",Status=" << status
				<< ",work_order.individual_index = " << individual_index
				<< ",work_order.example_case=" << example_case
				<< std::endl;
		}
	};

}