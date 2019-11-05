#include "SystemInfo.h"

#include <iostream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <windows.h>
#include "MyException.h"
#include "..\Domain\Arguments.h"

namespace Utilities
{
	// Purpose: 
	//   Return current CPU Temperature
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   Returns the current CPU Temperature 
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Unknown
	//
	// Remarks:
	//   This function calls the GetCpuTemperature function
	//
	double GetCpuTemperature()
	{
		double temperature = (std::numeric_limits<double>::max)();
		
		std::array<char, 128> buffer;
		std::string result;
		std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(domain::argmap::GetCpuTemperatureCmd.c_str(), "r"), _pclose);

		if (!pipe) 
		{
			std::stringstream error("GetCpuTemperature() - ");
			
			error << "popen() failed";
			
			std::cerr << error.str() << std::endl;
			throw MyException(error);
		}

		while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
		{
			result += buffer.data();
		}

		temperature = stod(result);

		return temperature;
	}

	// Purpose: 
	//   Return percent of memory in use
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   Returns the percent of system memory in use
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Unknown
	//
	// Remarks:
	//   This function calls the Windows API
	//
	unsigned long  GetMemoryLoad()
	{
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		return statex.dwMemoryLoad;
	}
}
