#include "SQLConnection.h"
#include "..\Utilities\Conversion.h"

namespace database
{
	HRESULT SQLConnection::initialize_and_establish_connection(const OLECHAR * server, const OLECHAR * dbString, const OLECHAR * userID, const OLECHAR * password)
	{
		IDBProperties   *pIDBProperties = nullptr;
		DBPROP          InitProperties[4] = { 0 };
		DBPROPSET       rgInitPropSet[1] = { 0 };
		HRESULT         hr = S_OK;

		try
		{
			// Obtain access to the OLE DB Driver for SQL Server.  
			hr = CoCreateInstance(CLSID_MSOLEDBSQL,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IDBInitialize,
				(void **)&pIDBInitialize_);
			if (FAILED(hr))
				throw MyException("Failed to obtain access to the OLE DB Driver.");

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
			InitProperties[1].vValue.bstrVal = SysAllocString(dbString);
			InitProperties[1].dwOptions = DBPROPOPTIONS_REQUIRED;
			InitProperties[1].colid = DB_NULLID;

			// Username (login).  
			//InitProperties[2].dwPropertyID = DBPROP_AUTH_INTEGRATED;
			//InitProperties[2].vValue.vt = VT_BSTR;
			//InitProperties[2].vValue.bstrVal = SysAllocString(L"SSPI");
			//InitProperties[2].dwOptions = DBPROPOPTIONS_REQUIRED;
			//InitProperties[2].colid = DB_NULLID;

			// Username (login).  
			InitProperties[2].dwPropertyID = DBPROP_AUTH_USERID;
			InitProperties[2].vValue.vt = VT_BSTR;
			InitProperties[2].vValue.bstrVal = SysAllocString(userID);
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
			hr = pIDBInitialize_->QueryInterface(IID_IDBProperties, (void **)&pIDBProperties);
			if (FAILED(hr))
				throw MyException("Failed to obtain an IDBProperties interface.");

			hr = pIDBProperties->SetProperties(1, rgInitPropSet);
			if (FAILED(hr))
				throw MyException("Failed to set initialization properties.");

			// Now establish the connection to the data source.  
			hr = pIDBInitialize_->Initialize();
			if (FAILED(hr))
				throw MyException("Failed to establish connection with the server.");
		}
		catch (MyException e)
		{
			if (pIDBProperties)
			{
				SysFreeString(InitProperties[0].vValue.bstrVal);
				SysFreeString(InitProperties[1].vValue.bstrVal);
				SysFreeString(InitProperties[2].vValue.bstrVal);
				SysFreeString(InitProperties[3].vValue.bstrVal);

				pIDBProperties->Release();
				pIDBProperties = nullptr;
			}

			if (FAILED(hr))
			{
				if (pIDBInitialize_)
				{
					pIDBInitialize_->Release();
					pIDBInitialize_ = nullptr;
				}
			}

			throw (e);
		}

		if (pIDBProperties)
		{
			SysFreeString(InitProperties[0].vValue.bstrVal);
			SysFreeString(InitProperties[1].vValue.bstrVal);
			SysFreeString(InitProperties[2].vValue.bstrVal);
			SysFreeString(InitProperties[3].vValue.bstrVal);

			pIDBProperties->Release();
			pIDBProperties = nullptr;
		}

		return hr;
	}

	SQLConnection::SQLConnection()
	{
		pIDBInitialize_ = NULL;
		pICommandText_ = NULL;
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
			hr = initialize_and_establish_connection(strtowstr(server).c_str(), strtowstr(dbString).c_str(), strtowstr(userID).c_str(), strtowstr(password).c_str());

			if (FAILED(hr))
				throw MyException("Failed to establish connection.");
		}
		catch (MyException e)
		{
			if (pIDBInitialize_)
			{
				pIDBInitialize_->Uninitialize();
				pIDBInitialize_->Release();
				pIDBInitialize_ = nullptr;
			}

			throw (e);
		}
	}

	void SQLConnection::disconnect()
	{
		// Free Up All Allocated Memory
		if (pIDBInitialize_)
		{
			pIDBInitialize_->Uninitialize();
			pIDBInitialize_->Release();
			pIDBInitialize_ = nullptr;
		}

		// Release The Component Object Module Library
		CoUninitialize();
	}
}
