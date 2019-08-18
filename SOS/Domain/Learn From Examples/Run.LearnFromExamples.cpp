#include "Run.LearnFromExamples.h"
#include "AsyncErrorFunction.LearnFromExample.h"
#include "ErrorFunction.LearnFromExample.h"
#include "../../Database/SQLCommand.h"
#include "../../Database/SQLField.h"
#include "../../PushGP/Globals.h"
#include "../../PushGP/Random.h"
#include "../../PushGP/Breed.h"

#include "..\..\PushP\ExecInstruction.h"

#include <functional>
#include <limits>

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

			// Create thread factories
			Push::intLiteralFactory = new Push::LiteralFactory<int>();
			Push::floatLiteralFactory = new Push::LiteralFactory<double>();
			Push::boolLiteralFactory = new Push::LiteralFactory<bool>();
			Push::codeListFactory = new Push::CodeListFactory();
			Push::doRangeClassFactory = new Push::DoRangeClassFactory();

			// Setup
			Push::init_push();

			for (int n = start_; n < argmap::population_size; n++)
			{
				pushGP::globals::population_agents[n].set_genome(pushGP::random_plush_genome());

				agents_created++;
			}

			// Cleanup thread factories
			Push::env.clear_stacks();

			delete Push::intLiteralFactory;
			delete Push::floatLiteralFactory;
			delete Push::boolLiteralFactory;
			delete Push::codeListFactory;
			delete Push::doRangeClassFactory;

			return agents_created;
		}

		void save_generation()
		{
			database::SQLCommand* sqlcmd_delete_indiciduals;
			database::SQLCommand* sqlcmd_insert_new_individual;

			sqlcmd_delete_indiciduals = new database::SQLCommand(&con, sqlstmt_delete_individuals);
			sqlcmd_insert_new_individual = new database::SQLCommand(&con);

			// Begin a transaction
			sqlcmd_insert_new_individual->begin_transaction();  //transaction->begin();

			// Delete previously saved generation
			sqlcmd_insert_new_individual->execute(sqlstmt_delete_individuals);

			// Save new generation
			sqlcmd_insert_new_individual->set_command(sqlstmt_insert_new_individual);

			for (int n = 0; n < argmap::population_size; n++)
			{
				sqlcmd_insert_new_individual->set_as_string(1, pushGP::globals::population_agents[n]);
				sqlcmd_insert_new_individual->execute();
			}

			// Commit transaction
			sqlcmd_insert_new_individual->commit_transaction();  //transaction->commit();

			delete sqlcmd_delete_indiciduals;
			delete sqlcmd_insert_new_individual;
		}

		void produce_new_offspring(std::function<double(static unsigned int _individual_index, static std::forward_list<int>& _input_list, static std::forward_list<int>& _output_list)> _run_individual_program,
			int _number_of_test_cases,
			std::forward_list<int> _test_cases_input[],
			std::forward_list<int> _test_cases_output[])
		{
			std::set<std::string> set_of_gnomes;
			std::pair<std::set<std::string>::iterator, bool> ret;

			// Reset children.
			for (unsigned int n = 0; n < argmap::population_size; n++)
				pushGP::globals::child_agents[n].clear_genome();

			for (unsigned int n = 0; n < argmap::population_size; n++)
			{
				if (pushGP::globals::child_agents[n].get_genome().empty())
				{
					pushGP::breed(pushGP::globals::child_agents[n],
						_run_individual_program,
						_number_of_test_cases,
						_test_cases_input,
						_test_cases_output);

					ret = set_of_gnomes.insert(pushGP::globals::child_agents[n].get_genome_as_string());

					// If a child with the same genome already exists, create a new random child.
					if (ret.second == false)
						pushGP::globals::child_agents[n].set_genome(pushGP::random_plush_genome());
				}
			}

			std::cout << std::endl;
		}

		void install_next_generation()
		{
			for (unsigned int n = 0; n < argmap::population_size; n++)
				pushGP::globals::population_agents[n].set(pushGP::globals::child_agents[n]);
		}

		int run(int argc, char** argv)
		{
			try
			{
				unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				unsigned int agents_created = 0;
				bool done = false;

				std::forward_list<int> test_cases_input[2];
				std::forward_list<int> test_cases_output[2];

				std::forward_list<int> test_case_input_1;
				test_case_input_1.push_front(1);
				test_case_input_1.push_front(4);
				test_case_input_1.push_front(2);
				test_case_input_1.push_front(9);
				test_case_input_1.push_front(10);
				test_case_input_1.push_front(5);

				std::forward_list<int> test_case_output_1;
				test_case_output_1.push_front(10);
				test_case_output_1.push_front(9);
				test_case_output_1.push_front(4);
				test_case_output_1.push_front(2);
				test_case_output_1.push_front(1);
				test_case_output_1.push_front(5);

				std::forward_list<int> test_case_input_2;
				test_case_input_2.push_front(7);
				test_case_input_2.push_front(9);
				test_case_input_2.push_front(12);
				test_case_input_2.push_front(15);
				test_case_input_2.push_front(14);
				test_case_input_2.push_front(3);
				test_case_input_2.push_front(2);
				test_case_input_2.push_front(7);

				std::forward_list<int> test_case_output_2;
				test_case_output_2.push_front(15);
				test_case_output_2.push_front(14);
				test_case_output_2.push_front(12);
				test_case_output_2.push_front(9);
				test_case_output_2.push_front(7);
				test_case_output_2.push_front(3);
				test_case_output_2.push_front(2);
				test_case_output_2.push_front(7);

				test_cases_input[0] = test_case_input_1;
				test_cases_input[1] = test_case_input_2;

				test_cases_output[0] = test_case_output_1;
				test_cases_output[1] = test_case_output_2;

				int number_of_test_cases = 2;

				// Initialize database connection
				con.connect(argmap::db_init_datasource, argmap::db_init_catalog, argmap::db_user_id, argmap::db_user_password);

				// Load population.  Create more if not enough loaded.
				std::cout << "Create Population Agents" << std::endl;
				generation_number = get_last_saved_generation_number() + 1;
				agents_created = make_pop_agents(load_pop_agents());

				if (agents_created > 0)
					generation_number = 0;

				while ((!done) && (generations_completed_this_session < argmap::max_generations_in_one_session))
				{
					std::cout << "Generation " << generation_number << std::endl;
					std::cout << "Session " << generations_completed_this_session << std::endl;
					save_generation();

					std::cout << "Produce New Offspring" << std::endl;
					produce_new_offspring(run_individual_program,
						number_of_test_cases,
						test_cases_input,
						test_cases_output);
				
					std::cout << "Install New Generation" << std::endl;
					install_next_generation();
					generation_number++;
					generations_completed_this_session++;
				}
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
