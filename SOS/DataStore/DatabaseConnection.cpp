#include "DatabaseConnection.h"
#include "..\Domain\Arguments.h"

namespace datastore
{
	DatabaseConnection::DatabaseConnection()
	{
		// Initialize database connection
		con.connect(domain::argmap::db_init_datasource, domain::argmap::db_init_catalog, domain::argmap::db_user_id, domain::argmap::db_user_password);
	}
}
