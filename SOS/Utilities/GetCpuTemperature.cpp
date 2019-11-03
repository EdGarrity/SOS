#include "GetCpuTemperature.h"

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include <limits>
#include <sstream>
#include <iomanip> 
#include "MyException.h"

#pragma comment(lib, "wbemuuid.lib")

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
	//   See http://www.cplusplus.com/forum/general/146576/
	double GetCpuTemperature()
	{
		double Temperature = (std::numeric_limits<double>::min)();

		HRESULT ci = CoInitialize(NULL);
		HRESULT hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

		if (FAILED(hr))
		{
			std::stringstream error("GetCpuTemperature() - ");

			error << "CoInitializeSecurity returned failure code ";
			error << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << hr;

			std::cerr << error.str() << std::endl;
			throw MyException(error);
		}

		IWbemLocator *pLocator;
		hr = CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLocator);

		if (FAILED(hr))
		{
			std::stringstream error("GetCpuTemperature() - ");

			error << "CoCreateInstance returned failure code ";
			error << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << hr;

			std::cerr << error.str() << std::endl;
			throw MyException(error);
		}

		IWbemServices *pServices;
		BSTR ns = SysAllocString(L"root\\WMI");
		hr = pLocator->ConnectServer(ns, NULL, NULL, NULL, 0, NULL, NULL, &pServices);
		pLocator->Release();
		SysFreeString(ns);

		if (FAILED(hr))
		{
			if (ci == S_OK)
				CoUninitialize();

			std::stringstream error("GetCpuTemperature() - ");

			error << "ConnectServer returned failure code ";
			error << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << hr;

			std::cerr << error.str() << std::endl;
			throw MyException(error);
		}

		BSTR query = SysAllocString(L"SELECT * FROM MSAcpi_ThermalZoneTemperature");
		BSTR wql = SysAllocString(L"WQL");
		IEnumWbemClassObject *pEnum;
		hr = pServices->ExecQuery(wql, query, WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);
		SysFreeString(wql);
		SysFreeString(query);
		pServices->Release();

		// See https://docs.microsoft.com/en-us/dotnet/framework/unmanaged-api/wmi/execquerywmi for error codes
		if (FAILED(hr))
		{
			if (ci == S_OK)
				CoUninitialize();

			std::stringstream error("GetCpuTemperature() - ");

			error << "ExecQuery returned failure code ";
			error << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << hr;

			std::cerr << error.str() << std::endl;
			throw MyException(error);
		}

		IWbemClassObject *pObject;
		ULONG returned;
		hr = pEnum->Next(WBEM_INFINITE, 1, &pObject, &returned);
		pEnum->Release();

		if (FAILED(hr))
		{
			if (ci == S_OK)
				CoUninitialize();

			std::stringstream error("GetCpuTemperature() - ");

			error << "pEnum->Next returned failure code ";
			error << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << hr;

			std::cerr << error.str() << std::endl;
			throw MyException(error);
		}

		BSTR temp = SysAllocString(L"CurrentTemperature");
		VARIANT v;
		VariantInit(&v);
		hr = pObject->Get(temp, 0, &v, NULL, NULL);
		pObject->Release();
		SysFreeString(temp);

		if (SUCCEEDED(hr))
			Temperature = (double)V_I4(&v) / 10.0 - 273.15;

		VariantClear(&v);

		if (ci == S_OK)
			CoUninitialize();

		return Temperature;
	}
}
