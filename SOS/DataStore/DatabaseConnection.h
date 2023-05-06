#include "..\Database\SQLConnection.h"
#include "..\Database\SQLCommand.h"

namespace datastore
{
	class DatabaseConnection
	{
	protected:
		database::SQLConnection con;

	public:
		DatabaseConnection();
		~DatabaseConnection() {};
	};
}
