#include <chrono>
#include <iostream>
#include <thread>
#include "SQLConnection.h"
#include "..\Utilities\Conversion.h"
//#include "..\Utilities\EventLogManager.h"

namespace database
{
	HRESULT SQLConnection::initialize_and_establish_connection(const OLECHAR * server, const OLECHAR * db_string, const OLECHAR * user_id, const OLECHAR * password)
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
				hr = CoCreateInstance(CLSID_SQLNCLI11,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_IDBInitialize,
					(void **)&pIDBInitialize_);
				
				if (FAILED(hr))
				{
					char error_message[80];
					sprintf_s(error_message, 80, "Attempt %d.  Failed to obtain access to the OLE DB Driver.  hr = %lX", attempts, hr);

					Utilities::quick_log << error_message << Utilities::endl;

					std::this_thread::sleep_for(std::chrono::milliseconds(500));

					if (attempts == 3)
						throw std::runtime_error(error_message);
				}
			}
			while (FAILED(hr));

			// Initialize property values needed to establish connection.  
			for (int i = 0; i < 4; i++)
				VariantInit(&InitProperties[i].vValue);

			// Server name.  
			// See DBPROP structure for more information on InitProperties  
			InitProperties[0].dwPropertyID = DBPROP_INIT_DATASOURCE;
			InitProperties[0].vValue.vt = VT_BSTR;
			InitProperties[0].vValue.bstrVal = SysAllocString(server);
			InitProperties[0].dwOptions = DBPROPOPTIONS_REQUIRED;
			InitProperties[0].colid = DB_NULLID;

			OLECHAR c;

			// Database.  
			InitProperties[1].dwPropertyID = DBPROP_INIT_CATALOG;
			InitProperties[1].vValue.vt = VT_BSTR;
			InitProperties[1].vValue.bstrVal = SysAllocString(db_string);
			InitProperties[1].dwOptions = DBPROPOPTIONS_REQUIRED;
			InitProperties[1].colid = DB_NULLID;

			// Username (login).  
			InitProperties[2].dwPropertyID = DBPROP_AUTH_USERID;
			InitProperties[2].vValue.vt = VT_BSTR;
			InitProperties[2].vValue.bstrVal = SysAllocString(user_id);
			InitProperties[2].dwOptions = DBPROPOPTIONS_REQUIRED;
			InitProperties[2].colid = DB_NULLID;

			// Password (login)
			InitProperties[3].dwPropertyID = DBPROP_AUTH_PASSWORD;
			InitProperties[3].vValue.vt = VT_BSTR;
			InitProperties[3].vValue.bstrVal = SysAllocString(password);
			InitProperties[3].dwOptions = DBPROPOPTIONS_REQUIRED;
			InitProperties[3].colid = DB_NULLID;

			// Construct the DBPROPSET structure(rgInitPropSet). The   
			// DBPROPSET structure is used to pass an array of DBPROP   
			// structures (InitProperties) to the SetProperties method.  
			rgInitPropSet[0].guidPropertySet = DBPROPSET_DBINIT;
			rgInitPropSet[0].cProperties = 4;
			rgInitPropSet[0].rgProperties = InitProperties;

			// Set initialization properties.  
			hr = pIDBInitialize_->QueryInterface(IID_IDBProperties, (void **)&pIDBProperties_);
			if (FAILED(hr))
				throw std::runtime_error("Failed to obtain an IDBProperties interface.");

			hr = pIDBProperties_->SetProperties(1, rgInitPropSet);
			if (FAILED(hr))
				throw std::runtime_error("Failed to set initialization properties.");

			SysFreeString(InitProperties[0].vValue.bstrVal);
			SysFreeString(InitProperties[1].vValue.bstrVal);
			SysFreeString(InitProperties[2].vValue.bstrVal);
			SysFreeString(InitProperties[3].vValue.bstrVal);

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

			hr = initialize_and_establish_connection(strtowstr(server).c_str(),
				strtowstr(dbString).c_str(),
				strtowstr(userID).c_str(),
				strtowstr(password).c_str());

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
			//Utilities::log_warning("SQLConnection::disconnect() - Error freeeing resources");
			Utilities::quick_log << "SQLConnection::disconnect() - Error freeeing resources" << Utilities::endl;

		}

		// Release The Component Object Module Library
		CoUninitialize();
	}
}
