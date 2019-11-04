// SMCoreTempCPPReader.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include "stdafx.h"
#include "GetCpuTemperature.h"
#include "GetCoreTempInfo.h"

//void CALLBACK		RerfreshInfoProc(HWND hwnd, UINT message, WPARAM idEvent, DWORD dwTime);
//DWORD WINAPI		ThreadProc(LPVOID lpParameter);
void				GetCoreTempInfoAlternative();
//void				cls();

UINT_PTR uiTimerPtr;

int _tmain(int argc, _TCHAR* argv[])
{
	//Setup a new thread to initiate a timer and send messages.
//	HANDLE hThread = CreateThread(NULL, NULL, ThreadProc, NULL, NULL, NULL);

	//Manually get Core Temp data the first time.
//	RerfreshInfoProc(NULL, WM_TIMER, 0, 0);

	GetCoreTempInfoAlternative();

	//Press return to exit.
	//char *c = new char;
	//scanf_s(c);
	//delete c;

	//Cleanup	
	//KillTimer(NULL, uiTimerPtr);
	//CloseHandle(hThread);

	return 0;
}

//This procedure will get and print data from "GetCoreTempInfo.dll" to the screen.
//void CALLBACK		RerfreshInfoProc(HWND hwnd, UINT message, WPARAM idEvent, DWORD dwTime)
//{
//	double temp = 10000000;
//	ULONG index;
//	char tempType;
//	CoreTempProxy coreTempProxy;
//
//	if (coreTempProxy.GetData())
//	{
//		tempType = coreTempProxy.IsFahrenheit() ? 'F' : 'C';
//
//		for (UINT i = 0; i < coreTempProxy.GetCPUCount(); i++)
//		{
//			for (UINT g = 0; g < coreTempProxy.GetCoreCount(); g++)
//			{
//				index = g + (i * coreTempProxy.GetCoreCount());
//				temp = (temp < coreTempProxy.GetTemp(index)) ? temp : coreTempProxy.GetTemp(index);
//			}
//		}
//
//		std::cout << temp << std::endl;
//	}
//	else
//	{
//		//Display DLL related errors.
//		std::stringstream error;
//
//		error << "Error: Core Temp's shared memory could not be read." << std::endl;
//		error << "Error number: " << coreTempProxy.GetDllError() << std::endl;
//		error << "Error description: " << coreTempProxy.GetErrorMessage() << std::endl;
//
//		std::cerr << error.str() << std::endl;
//	}
//}

///
/// The following peice of code demonstrates how you could load the DLL dynamically at run time
/// This sample demonstrates the use of a function to retrieve the data from the DLL rather than
/// a proxy class like in the sample above, this can be used by C users or other languages.
///

typedef bool (WINAPI *myGetCoreTempInfo)(CORE_TEMP_SHARED_DATA *pData);

void GetCoreTempInfoAlternative()
{
	double temp = -10000000;
	myGetCoreTempInfo GetCoreTempInfo;
	HMODULE hCT;
	ULONG index;
	DWORD lastError = 0;
	wchar_t errMsg[100];
	char tempType;
	memset(errMsg, 0, sizeof(errMsg));
	CORE_TEMP_SHARED_DATA *CoreTempData = new CORE_TEMP_SHARED_DATA;
	memset(CoreTempData, 0, sizeof(CORE_TEMP_SHARED_DATA));

	//Load DLL.
	hCT = LoadLibrary(TEXT("GetCoreTempInfo.dll"));
	if (hCT)
	{
		//Get the address of the function.
		GetCoreTempInfo = (myGetCoreTempInfo)GetProcAddress(hCT, "fnGetCoreTempInfoAlt");
		if (GetCoreTempInfo)
		{
			//Call the function, if it returns true continue to print the info.
			if (GetCoreTempInfo(CoreTempData))
			{
				tempType = CoreTempData->ucFahrenheit ? 'F' : 'C';

				for (UINT i = 0; i < CoreTempData->uiCPUCnt; i++)
				{
					for (UINT g = 0; g < CoreTempData->uiCoreCnt; g++)
					{
						index = g + (i * CoreTempData->uiCoreCnt);
						temp = (temp > CoreTempData->fTemp[index]) ? temp : CoreTempData->fTemp[index];
					}
				}
				
				std::cout << temp << std::endl;
			}
			else
			{
				//Display DLL related errors.
				//lastError = GetLastError();
				//printf("Error: Core Temp's shared memory could not be read.\n");
				//printf("Error number: %d\n", GetLastError());
				//if ((lastError & UNKNOWN_EXCEPTION) > 0)
				//{
				//	printf("Error description: Unknown error occured while copying shared memory.\n");
				//}
				//else
				//{
				//	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
				//		NULL, lastError, 0, errMsg, 100, NULL);
				//	wprintf(TEXT("Error description: %s\n"), errMsg);
				//}

				//Display DLL related errors.
				lastError = GetLastError();
				std::stringstream error;
				
				error << "Error: Core Temp's shared memory could not be read." << std::endl;
				error << "Error number: " << GetLastError() << std::endl;

				if ((lastError & UNKNOWN_EXCEPTION) > 0)
					error << "Error description : Unknown error occured while copying shared memory." << std::endl;
				
				else
				{
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, lastError, 0, errMsg, 100, NULL);
					error << "Error description: " << errMsg << std::endl;
				}

				std::cerr << error.str() << std::endl;
			}
		}
		else
			std::cerr << "Error: The function \"fnGetCoreTempInfo\" in \"GetCoreTempInfo.dll\" could not be found." << std::endl;

		FreeLibrary(hCT);
		hCT = NULL;
	}
	else
		std::cerr << "Error: \"GetCoreTempInfo.dll\" could not be loaded." << std::endl;

	//Free resources.
	delete CoreTempData;
}

//This thread will setup our timer and handle the timer messages.
//DWORD WINAPI ThreadProc(LPVOID lpParameter)
//{
//	uiTimerPtr = SetTimer(NULL, 0, 50, (TIMERPROC)RerfreshInfoProc);
//
//	MSG msg;
//	//while (GetMessage(&msg, NULL, 0, 0))
//	//{
//	//	// TranslateMessage(&msg);
//	//	DispatchMessage(&msg);
//	//}
//
//	GetMessage(&msg, NULL, 0, 0);
//	DispatchMessage(&msg);
//
//	return 0;
//}
//
//Clear screen function from MSDN.
//void cls()
//{
//	HANDLE	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	COORD	coordScreen = { 0, 0 };		/* here's where we'll home the cursor */
//	BOOL	bSuccess;
//	DWORD	cCharsWritten;
//	CONSOLE_SCREEN_BUFFER_INFO	csbi;	/* to get buffer info */
//	DWORD	dwConSize;				/* number of character cells in the current buffer */
//	/* get the number of character cells in the current buffer */
//	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
//	if (!bSuccess) return;
//	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
//	/* fill the entire screen with blanks */
//	bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
//	if (!bSuccess) return;
//	/* get the current text attribute */
//	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
//	if (!bSuccess) return;
//	/* now set the buffer's attributes accordingly */
//	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
//	if (!bSuccess) return;
//	/* put the cursor at (0, 0) */
//	bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
//	if (!bSuccess) return;
//	return;
//}
