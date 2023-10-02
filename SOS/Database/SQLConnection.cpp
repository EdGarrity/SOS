#include <chrono>
#include <iostream>
#include <thread>
#include "SQLConnection.h"
#include "..\Utilities\Conversion.h"
//#include "..\Utilities\EventLogManager.h"
#include "..\Utilities\Debug.h"
#include "..\Utilities\Conversion.h"

namespace database
{
	//HRESULT SQLConnection::initialize_and_establish_connection(const OLECHAR * server, const OLECHAR * db_string, const OLECHAR * user_id, const OLECHAR * password)
	//{
	//	DBPROP          InitProperties[5] = { 0 };
	//	DBPROPSET       rgInitPropSet[1] = { 0 };
	//	HRESULT         hr = S_OK;

	//	try
	//	{
	//		int attempts = 0;

	//		do
	//		{
	//			attempts++;

	//			// Obtain access to the OLE DB Driver for SQL Server.  
	//			hr = CoCreateInstance(MSOLEDBSQL_CLSID,
	//				NULL,
	//				CLSCTX_INPROC_SERVER,
	//				IID_IDBInitialize,
	//				(void **)&pIDBInitialize_);
	//			
	//			if (FAILED(hr))
	//			{
	//				char error_message[80];
	//				sprintf_s(error_message, 80, "Attempt %d.  Failed to obtain access to the OLE DB Driver.  hr = %lX", attempts, hr);

	//				std::cout << error_message << std::endl;

	//				std::this_thread::sleep_for(std::chrono::milliseconds(500));

	//				if (attempts == 3)
	//					throw std::runtime_error(error_message);
	//			}
	//		}
	//		while (FAILED(hr));

	//		// Initialize property values needed to establish connection.  
	//		for (int i = 0; i < 5; i++)
	//			VariantInit(&InitProperties[i].vValue);

	//		// Server name.  
	//		// See DBPROP structure for more information on InitProperties  
	//		InitProperties[0].dwPropertyID = DBPROP_INIT_DATASOURCE;
	//		InitProperties[0].vValue.vt = VT_BSTR;
	//		InitProperties[0].vValue.bstrVal = SysAllocString(server);
	//		InitProperties[0].dwOptions = DBPROPOPTIONS_REQUIRED;
	//		InitProperties[0].colid = DB_NULLID;

	//		OLECHAR c = '\0';

	//		// Database.  
	//		InitProperties[1].dwPropertyID = DBPROP_INIT_CATALOG;
	//		InitProperties[1].vValue.vt = VT_BSTR;
	//		InitProperties[1].vValue.bstrVal = SysAllocString(db_string);
	//		InitProperties[1].dwOptions = DBPROPOPTIONS_REQUIRED;
	//		InitProperties[1].colid = DB_NULLID;

	//		// Username (login).  
	//		InitProperties[2].dwPropertyID = DBPROP_AUTH_USERID;
	//		InitProperties[2].vValue.vt = VT_BSTR;
	//		InitProperties[2].vValue.bstrVal = SysAllocString(user_id);
	//		InitProperties[2].dwOptions = DBPROPOPTIONS_REQUIRED;
	//		InitProperties[2].colid = DB_NULLID;

	//		// Password (login)
	//		InitProperties[3].dwPropertyID = DBPROP_AUTH_PASSWORD;
	//		InitProperties[3].vValue.vt = VT_BSTR;
	//		InitProperties[3].vValue.bstrVal = SysAllocString(password);
	//		InitProperties[3].dwOptions = DBPROPOPTIONS_REQUIRED;
	//		InitProperties[3].colid = DB_NULLID;

	//		// Encrypt=Optional
	//		const OLECHAR* encryption = L"Optional";
	//		InitProperties[4].dwPropertyID = SSPROP_INIT_ENCRYPT;
	//		InitProperties[4].vValue.vt = VT_BSTR;
	//		InitProperties[4].vValue.bstrVal = SysAllocString(encryption);
	//		InitProperties[4].dwOptions = DBPROPOPTIONS_REQUIRED;
	//		InitProperties[4].colid = DB_NULLID;

	//		// Construct the DBPROPSET structure(rgInitPropSet). The   
	//		// DBPROPSET structure is used to pass an array of DBPROP   
	//		// structures (InitProperties) to the SetProperties method.  
	//		rgInitPropSet[0].guidPropertySet = DBPROPSET_DBINIT;
	//		rgInitPropSet[0].cProperties = 5;
	//		rgInitPropSet[0].rgProperties = InitProperties;

	//		// Set initialization properties.  
	//		hr = pIDBInitialize_->QueryInterface(IID_IDBProperties, (void **)&pIDBProperties_);
	//		if (FAILED(hr))
	//			throw std::runtime_error("Failed to obtain an IDBProperties interface.");

	//		hr = pIDBProperties_->SetProperties(1, rgInitPropSet);
	//		if (FAILED(hr))
	//			throw std::runtime_error("Failed to set initialization properties.");

	//		SysFreeString(InitProperties[0].vValue.bstrVal);
	//		SysFreeString(InitProperties[1].vValue.bstrVal);
	//		SysFreeString(InitProperties[2].vValue.bstrVal);
	//		SysFreeString(InitProperties[3].vValue.bstrVal);
	//		SysFreeString(InitProperties[4].vValue.bstrVal);

	//		pIDBProperties_->Release();
	//		pIDBProperties_ = nullptr;

	//		// Now establish the connection to the data source.  
	//		hr = pIDBInitialize_->Initialize();
	//		if (FAILED(hr))
	//			throw std::runtime_error("Failed to establish connection with the server.");
	//	}
	//	catch (...)
	//	{
	//		if (pIDBProperties_)
	//		{
	//			SysFreeString(InitProperties[0].vValue.bstrVal);
	//			SysFreeString(InitProperties[1].vValue.bstrVal);
	//			SysFreeString(InitProperties[2].vValue.bstrVal);
	//			SysFreeString(InitProperties[3].vValue.bstrVal);
	//			SysFreeString(InitProperties[4].vValue.bstrVal);

	//			pIDBProperties_->Release();
	//			pIDBProperties_ = nullptr;
	//		}

	//		if (FAILED(hr))
	//		{
	//			if (pIDBInitialize_)
	//			{
	//				pIDBInitialize_->Release();
	//				pIDBInitialize_ = nullptr;
	//			}
	//		}

	//		std::throw_with_nested(std::runtime_error("SQLConnection::initialize_and_establish_connection()"));
	//	}

	//	if (pIDBProperties_)
	//	{
	//		SysFreeString(InitProperties[0].vValue.bstrVal);
	//		SysFreeString(InitProperties[1].vValue.bstrVal);
	//		SysFreeString(InitProperties[2].vValue.bstrVal);
	//		SysFreeString(InitProperties[3].vValue.bstrVal);
	//		SysFreeString(InitProperties[4].vValue.bstrVal);
	//	}

	//	return hr;
	//}

	HRESULT SQLConnection::initialize_and_establish_connection(LPCWSTR lpwszProviderString)
	{
		DBPROP          InitProperties[4] = { 0 };
		DBPROPSET       rgInitPropSet[1] = { 0 };
		HRESULT         hr = S_OK;

		try
		{
			int attempts = 0;

			do
			{
				attempts++;

				// Obtain access to the OLE DB Driver for SQL Server.  
				hr = CoCreateInstance(MSOLEDBSQL_CLSID,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_IDBInitialize,
					(void**)&pIDBInitialize_);

				if (FAILED(hr))
				{
					char error_message[80];
					sprintf_s(error_message, 80, "Attempt %d.  Failed to obtain access to the OLE DB Driver.  hr = %lX", attempts, hr);

					std::cout << error_message << std::endl;

					std::this_thread::sleep_for(std::chrono::milliseconds(500));

					if (attempts == 3)
						throw std::runtime_error(error_message);
				}
			} while (FAILED(hr));

			// Initialize property values needed to establish connection.  
			for (int i = 0; i < 4; i++)
				VariantInit(&InitProperties[i].vValue);

			// Server name.  
			// See DBPROP structure for more information on InitProperties  
			InitProperties[0].dwPropertyID = DBPROP_INIT_PROVIDERSTRING;
			InitProperties[0].vValue.vt = VT_BSTR;
			InitProperties[0].vValue.bstrVal = SysAllocString(lpwszProviderString);
			InitProperties[0].dwOptions = DBPROPOPTIONS_REQUIRED;
			InitProperties[0].colid = DB_NULLID;

			// Construct the DBPROPSET structure(rgInitPropSet). The   
			// DBPROPSET structure is used to pass an array of DBPROP   
			// structures (InitProperties) to the SetProperties method.  
			rgInitPropSet[0].guidPropertySet = DBPROPSET_DBINIT;
			rgInitPropSet[0].cProperties = 1;
			rgInitPropSet[0].rgProperties = InitProperties;

			// Set initialization properties.  
			hr = pIDBInitialize_->QueryInterface(IID_IDBProperties, (void**)&pIDBProperties_);
			if (FAILED(hr))
				throw std::runtime_error("Failed to obtain an IDBProperties interface.");

			hr = pIDBProperties_->SetProperties(1, rgInitPropSet);
			if (FAILED(hr))
				throw std::runtime_error("Failed to set initialization properties.");

			SysFreeString(InitProperties[0].vValue.bstrVal);

			pIDBProperties_->Release();
			pIDBProperties_ = nullptr;

			// Now establish the connection to the data source.  
			hr = pIDBInitialize_->Initialize();
			if (FAILED(hr))
				throw std::runtime_error("Failed to establish connection with the server.");
		}
		catch (...)
		{
			if (pIDBProperties_)
			{
				SysFreeString(InitProperties[0].vValue.bstrVal);
				SysFreeString(InitProperties[1].vValue.bstrVal);
				SysFreeString(InitProperties[2].vValue.bstrVal);
				SysFreeString(InitProperties[3].vValue.bstrVal);

				pIDBProperties_->Release();
				pIDBProperties_ = nullptr;
			}

			if (FAILED(hr))
			{
				if (pIDBInitialize_)
				{
					pIDBInitialize_->Release();
					pIDBInitialize_ = nullptr;
				}
			}

			std::throw_with_nested(std::runtime_error("SQLConnection::initialize_and_establish_connection()"));
		}

		if (pIDBProperties_)
		{
			SysFreeString(InitProperties[0].vValue.bstrVal);
			SysFreeString(InitProperties[1].vValue.bstrVal);
			SysFreeString(InitProperties[2].vValue.bstrVal);
			SysFreeString(InitProperties[3].vValue.bstrVal);
		}

		return hr;
	}

	SQLConnection::SQLConnection()
	{
		pIDBInitialize_ = NULL;
	}

	SQLConnection::SQLConnection(const std::string server, const std::string dbString, const std::string userID, const std::string password)
	{
		connect(server, dbString, userID, password);
	}

	SQLConnection::~SQLConnection()
	{
		disconnect();
	}

	void SQLConnection::connect(const std::string server, const std::string dbString, const std::string userID, const std::string password)
	{
		HRESULT             hr = S_OK;

		// Initialize the Component Object Module Library
		CoInitialize(nullptr);

		try
		{
			//hr = initialize_and_establish_connection(Utilities::strtowstr(server).c_str(), 
			//	Utilities::strtowstr(dbString).c_str(), 
			//	Utilities::strtowstr(userID).c_str(), 
			//	Utilities::strtowstr(password).c_str());

			//hr = initialize_and_establish_connection(L"Server=(local);Database=CleverLogsData;UID=TACollection-Ingestor;PWD=CD-Ing@TAColl!;Encrypt=Optional");


			//std::string input; // initialized elsewhere
			//std::wstring output;

			//output = std::wstring(input.begin(), input.end());
			//output = output + std::wstring(L" program"); // or output += L" program";
			//const wchar_t* ptr = output.c_str();

			//std::wstring = std::wstring(server.begin(), server.end());


			//output = server << ";" << dbString << ";" << userID << ";" << password << ";Encrypt=Optional";

			//hr = initialize_and_establish_connection(output.c_str());


			std::wstring output = Utilities::strtowstr(server)
				+ Utilities::strtowstr(";").c_str()
				+ Utilities::strtowstr(dbString).c_str()
				+ Utilities::strtowstr(";").c_str()
				+ Utilities::strtowstr(userID).c_str()
				+ Utilities::strtowstr(";").c_str()
				+ Utilities::strtowstr(password).c_str()
				+ Utilities::strtowstr(";Encrypt=Optional").c_str();

			hr = initialize_and_establish_connection(output.c_str());

			if (FAILED(hr))
				throw std::runtime_error("Failed to establish connection.");
		}
		catch (...)
		{
			if (pIDBInitialize_)
			{
				pIDBInitialize_->Uninitialize();
				pIDBInitialize_->Release();
				pIDBInitialize_ = nullptr;
			}

			std::throw_with_nested(std::runtime_error("SQLConnection::initialize_and_establish_connection()"));
		}
	}

	bool SQLConnection::connected()
	{
		return pIDBInitialize_ != NULL;
	}

	void SQLConnection::disconnect()
	{
		// Free Up All Allocated Memory
		try
		{
			if (pIDBInitialize_)
			{
				pIDBInitialize_->Uninitialize();
				pIDBInitialize_->Release();
				pIDBInitialize_ = nullptr;
			}

			if (pIDBProperties_)
			{
				pIDBProperties_->Release();
				pIDBProperties_ = nullptr;
			}
		}
		catch (...)
		{
			std::ostringstream ss;
			ss << ",method=SQLConnection.disconnect"
				<< ",exception=Unknown"
				<< ",message=Error freeeing resources";
			Utilities::logline_threadsafe << ss.str();

			std::cerr << "SQLConnection::disconnect() - Error freeeing resources";
		}

		// Release The Component Object Module Library
		CoUninitialize();
	}
}
