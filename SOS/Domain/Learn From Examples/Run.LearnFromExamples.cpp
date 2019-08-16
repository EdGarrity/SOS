#include "Run.LearnFromExamples.h"
#include "../../Database/SQLCommand.h"
#include "../../Database/SQLField.h"
#include "../../PushGP/Globals.h"
#include "../../PushGP/Random.h"

namespace domain
{
	namespace learn_from_examples
	{
		database::SQLConnection con;

		const std::string sqlstmt_get_last_saved_generation_number = "SELECT TOP 1 [Generation] FROM [SOS].[dbo].[ProgressLog] ORDER BY[Update_DTS] DESC;";
		const std::string sqlstmt_sqlcmd_get_individuals = "SELECT [Genome] FROM [dbo].[Individuals];";
		const std::string sqlstmt_delete_individuals("DELETE FROM [SOS].[dbo].[Individuals];");
		const std::string sqlstmt_insert_new_individual("INSERT INTO [dbo].[Individuals] ([Genome]) VALUES (?);");

		unsigned long get_last_saved_generation_number()
		{
			unsigned long n = 0;

			database::SQLCommand* sqlcmd_get_last_saved_generation_number;

			sqlcmd_get_last_saved_generation_number = new database::SQLCommand(&con, sqlstmt_get_last_saved_generation_number);

			sqlcmd_get_last_saved_generation_number->execute();

			if (sqlcmd_get_last_saved_generation_number->fetch_next())
				n = sqlcmd_get_last_saved_generation_number->get_field_as_long(1);

			delete sqlcmd_get_last_saved_generation_number;

			return n;
		}

		unsigned int load_pop_agents()
		{
			unsigned int n = 0;

			database::SQLCommand* sqlcmd_get_individuals;

			sqlcmd_get_individuals = new database::SQLCommand(&con, sqlstmt_sqlcmd_get_individuals);

			try
			{
				sqlcmd_get_individuals->execute();

				if (sqlcmd_get_individuals->is_result_set())
				{
					while ((sqlcmd_get_individuals->fetch_next()) && (n < argmap::population_size))
					{
						std::cout << "n = " << n << std::endl;

						std::string genome = sqlcmd_get_individuals->get_field_as_string(1);
						pushGP::globals::population_agents[n].set_genome(genome);

						n++;
					}
				}
			}
			catch (...)
			{
				delete sqlcmd_get_individuals;

				return n;
			}

			delete sqlcmd_get_individuals;

			return n;
		}

		unsigned int make_pop_agents(int start_)
		{
			unsigned int agents_created = 0;

			for (int n = start_; n < argmap::population_size; n++)
			{
				pushGP::globals::population_agents[n].set_genome(pushGP::random_plush_genome());

				agents_created++;
			}

			return agents_created;
		}

		int run(int argc, char** argv)
		{
			try
			{
				unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				unsigned int agents_created = 0;
				bool done = false;

				// Initialize database connection
				con.connect(argmap::db_init_datasource, argmap::db_init_catalog, argmap::db_user_id, argmap::db_user_password);
			}
			catch (const std::exception& e)
			{
				std::cout << "Standard exception: " << e.what() << std::endl;
				throw;
			}
			catch (...)
			{
				std::cout << "Exception occurred" << std::endl;
				throw;
			}

			return 0;
		}
	}
}
