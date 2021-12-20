#pragma once

#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <time.h>
#include <ctime>
#include <queue>
#include <sstream>
#include <fstream>

#include "SystemInfo.h"
//#include "..\Plush\Atom.h"
//#include "..\Plush\Environment.h"

#define DLEVEL 0
#define TRACE_LEVEL 1

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

#if DLEVEL > 0
	inline void debug_log(const int env_index, std::string function, std::string status)
	{
		static std::string prev_status = "";

		//if ((prev_status != status) && (env_index == -1))
		//{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

			unsigned long percent_memory_use = GetMemoryLoad();

			std::cout << getCurrentTimestamp()
				<< ",LineNumber=" << std::to_string(line_number++)
				<< ",Percent_Free_Memory=" << std::to_string(percent_memory_use)
				<< ",Thread=" << env_index
				<< ",Function=" << function
				<< ",Status=" << status
				<< std::endl;

			work_order_print_lock.unlock();
		//}
	}

	inline void debug_log_nolock(const int env_index, std::string function, std::string status)
	{
		static std::string prev_status = "";

		//if ((prev_status != status) && (env_index == -1))
		//{
			prev_status = status;

			unsigned long percent_memory_use = GetMemoryLoad();

			std::cout << getCurrentTimestamp()
				<< ",LineNumber=" << std::to_string(line_number++)
				<< ",Percent_Free_Memory=" << std::to_string(percent_memory_use)
				<< ",Thread=" << env_index
				<< ",Function=" << function
				<< ",Status=" << status
				<< std::endl;
		//}
	}

	inline void debug_log(const int env_index,
		std::string function, 
		std::string status, 
		unsigned int individual_index, 
		unsigned int example_case)
	{
		static std::string prev_status = "";

		//if ((prev_status != status) && (env_index == -1))
		//{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

			unsigned long percent_memory_use = GetMemoryLoad();

			std::cout << getCurrentTimestamp()
				<< ",LineNumber=" << std::to_string(line_number++)
				<< ",Percent_Free_Memory=" << std::to_string(percent_memory_use)
				<< ",Thread=" << env_index
				<< ",Function=" << function
				<< ",Status=" << status
				<< ",work_order.individual_index = " << individual_index
				<< ",work_order.example_case=" << example_case
				<< std::endl;

			work_order_print_lock.unlock();
		//}
	};
#endif

#if TRACE_LEVEL>0
	extern std::queue<std::string> trace_queue;
	extern int generation;

	inline void trace_record(std::string trace_msg)
	{
		static bool first = true;
		std::basic_stringstream<char> ss;
		
		line_number++;

		//ss << getCurrentTimestamp()
		//	<< "line_number=" << line_number
		//	<< "generation=" << generation
		//	<< trace_msg
		//	<< std::endl;

		ss << getCurrentTimestamp()
			<< "," << line_number
			<< "," << generation
			<< trace_msg
			<< std::endl;

		std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

		std::ofstream myfile;
		myfile.open("c:\\temp\\trace.txt", std::ios::out | std::ios::app);

		if (first)
		{
			first = false;

			myfile << "time,line_number,generation,thread,individual_index,example_case,ip,instruction,enabled,Exec,Code,long,double,bool,instructions";
			myfile << std::endl;
		}

		myfile << ss.str();

		myfile.close();

		work_order_print_lock.unlock();
	};

	inline void flush_trace_queue()
	{
		//std::ostringstream buf("trace.txt", std::ostringstream::out);

		//while (trace_queue.empty() == false)
		//{
		//	buf << trace_queue.front();
		//	buf << std::endl;
		//	trace_queue.pop();
		//}

		generation++;
	}
#endif
}