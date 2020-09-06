#include <functional>
#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>
#include <deque>
#include <ppl.h>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include "Run.LearnFromExamples.h"
#include "..\..\Database\SQLCommand.h"
#include "..\..\PushGP\SimulatedAnnealing.h"
#include "..\..\Utilities\SystemInfo.h"
#include "..\..\Plush\Environment.h"
#include "..\..\Utilities\CSVIterator.h"
#include "../../Utilities/Random.Utilities.h"
#include "../../PushGP/Globals.h"
//#include "ErrorFunction.LearnFromExample.h"
//#include "../../PushGP/Random.h"
//#include "../../PushGP/Breed.h"
//#include "../../Plush/Environment.h"
//#include "../../PushGP/Utilities.h"
//#include "../../Plush/Genome.h"
//#include "../../Utilities/Conversion.h"
//#include "../../PushGP/AsyncBreed.h"

// Correction for Syntax error with std::numeric_limits::max compiler error
// See https://stackoverflow.com/questions/27442885/syntax-error-with-stdnumeric-limitsmax
#undef max

using namespace concurrency;

namespace domain
{
	namespace learn_from_examples
	{
		concurrent_vector<double> training_cases_problem[argmap::number_of_training_cases];
		concurrent_vector<double> training_cases_solution[argmap::number_of_training_cases];
		concurrent_vector<double> test_cases_problem[argmap::number_of_test_cases];
		concurrent_vector<double> test_cases_solution[argmap::number_of_test_cases];

		database::SQLConnection con;

		const std::string sqlstmt_get_last_saved_generation_number = "SELECT TOP 1 [Generation] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_saved_temperature = "SELECT TOP 1 [Tempareture] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_best_individual_error = "SELECT TOP 1 [BestIndividual_Training_Error] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_prev_best_individual_error = "SELECT TOP 1 [BestIndividual_Prev_Training_Error] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_stalled_count = "SELECT TOP 1 [Stalled_Count] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_cool_down_count = "SELECT TOP 1 [Cool_Down_Count] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_include_best_individual_in_breeding_pool = "SELECT TOP 1 [Include_Best_Individual_In_Breeding_Pool] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_sqlcmd_load_example_cases = "SELECT [Problem], [Solution] FROM [dbo].[ExampleCases];";
		const std::string sqlstmt_delete_all_example_cases("DELETE FROM [SOS].[dbo].[ExampleCases];");
		const std::string sqlstmt_insert_new_example_case("INSERT INTO [dbo].[ExampleCases] ([Problem], [Solution]) VALUES (?,?);");
		const std::string sqlstmt_sqlcmd_get_individuals = "SELECT [Individual_ID],"
			"[Genome], "
			"[Parent_1],"
			"[Parent_2],"
			"[Parent_1_1],"
			"[Parent_1_2],"
			"[Parent_2_1],"
			"[Parent_2_2],"
			"[Parent_1_1_1],"
			"[Parent_1_1_2],"
			"[Parent_1_2_1],"
			"[Parent_1_2_2],"
			"[Parent_2_1_1],"
			"[Parent_2_1_2],"
			"[Parent_2_2_1],"
			"[Parent_2_2_2] "
			"FROM [dbo].[Individuals] ORDER BY [Individual_ID];";
		const std::string sqlstmt_delete_individual("DELETE FROM [SOS].[dbo].[Individuals];");
		const std::string sqlstmt_insert_new_individual("INSERT INTO [dbo].[Individuals] ([Individual_ID], [Genome], [Parent_1], [Parent_2], [Parent_1_1], [Parent_1_2], [Parent_2_1], [Parent_2_2], [Parent_1_1_1], [Parent_1_1_2], [Parent_1_2_1], [Parent_1_2_2], [Parent_2_1_1], [Parent_2_1_2], [Parent_2_2_1], [Parent_2_2_2]) VALUES	(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		//                                                                     1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6

		const std::string sqlstmt_save_status_report("INSERT INTO [dbo].[ProgressLog]"
			"           ("
			"            [Generation]"								// 1
			"           ,[Generations_Completed_This_Session]"		// 2
			"           ,[BestIndividual_ID]"						// 3
			"           ,[BestIndividual_Training_Score]"			// 4
			"           ,[BestIndividual_Training_Error]"			// 5
			"           ,[Average_Training_Error]"					// 6
			"           ,[Standard_Deviation]"						// 7
			"           ,[BestIndividual_Test_Score]"				// 8
			"           ,[Number_Of_Training_Cases]"				// 9
			"           ,[Number_Of_Test_Cases]"					// 10
			"           ,[Best_Genome]"								// 11
			"           ,[Population_Size]"							// 12
			"           ,[Alternation_Rate]"						// 13
			"           ,[Uniform_Mutation_Rate]"					// 14
			"           ,[Example_Case_Max_Length]"					// 15
			"           ,[Example_Case_Upper_Range]"				// 16
			"           ,[Tempareture]"								// 17
			"           ,[BestIndividual_Prev_Training_Error]"		// 18
			"           ,[Stalled_Count]"							// 19
			"           ,[Cool_Down_Count]"							// 20
			"           ,[Include_Best_Individual_In_Breeding_Pool]"// 21
			"           )"
			"     VALUES"
			"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		//       1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1

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

		double get_last_saved_temperature(double _default_temperature)
		{
			double n = _default_temperature;

			database::SQLCommand* sqlcmd_get_last_saved_temperature;

			sqlcmd_get_last_saved_temperature = new database::SQLCommand(&con, sqlstmt_get_last_saved_temperature);

			sqlcmd_get_last_saved_temperature->execute();

			if (sqlcmd_get_last_saved_temperature->fetch_next())
				n = sqlcmd_get_last_saved_temperature->get_field_as_double(1);

			delete sqlcmd_get_last_saved_temperature;

			return n;
		}

		double get_last_best_individual_error(double _default_error)
		{
			double n = _default_error;

			database::SQLCommand* sqlcmd_get_last_best_individual_error;

			sqlcmd_get_last_best_individual_error = new database::SQLCommand(&con, sqlstmt_get_last_best_individual_error);

			sqlcmd_get_last_best_individual_error->execute();

			if (sqlcmd_get_last_best_individual_error->fetch_next())
				n = sqlcmd_get_last_best_individual_error->get_field_as_double(1);

			delete sqlcmd_get_last_best_individual_error;

			return n;
		}

		double get_last_prev_best_individual_error(double _default_error)
		{
			double n = _default_error;

			database::SQLCommand* sqlcmd_get_last_prev_best_individual_error;

			sqlcmd_get_last_prev_best_individual_error = new database::SQLCommand(&con, sqlstmt_get_last_prev_best_individual_error);

			sqlcmd_get_last_prev_best_individual_error->execute();

			if (sqlcmd_get_last_prev_best_individual_error->fetch_next())
				n = sqlcmd_get_last_prev_best_individual_error->get_field_as_double(1);

			delete sqlcmd_get_last_prev_best_individual_error;

			return n;
		}

		unsigned long get_last_stalled_count(unsigned long _default_stalled_count)
		{
			unsigned long n = _default_stalled_count;

			database::SQLCommand* sqlcmd_get_last_stalled_count;

			sqlcmd_get_last_stalled_count = new database::SQLCommand(&con, sqlstmt_get_last_stalled_count);

			sqlcmd_get_last_stalled_count->execute();

			if (sqlcmd_get_last_stalled_count->fetch_next())
				n = sqlcmd_get_last_stalled_count->get_field_as_long(1);

			delete sqlcmd_get_last_stalled_count;

			return n;
		}

		unsigned long get_last_cool_down_count(unsigned long _default_cool_down_count)
		{
			unsigned long n = _default_cool_down_count;

			database::SQLCommand* sqlcmd_get_last_cool_down_count;

			sqlcmd_get_last_cool_down_count = new database::SQLCommand(&con, sqlstmt_get_last_cool_down_count);

			sqlcmd_get_last_cool_down_count->execute();

			if (sqlcmd_get_last_cool_down_count->fetch_next())
				n = sqlcmd_get_last_cool_down_count->get_field_as_long(1);

			delete sqlcmd_get_last_cool_down_count;

			return n;
		}


		bool get_include_best_individual_in_breeding_pool(unsigned long _default_include_best_individual_in_breeding_pool)
		{
			bool n = _default_include_best_individual_in_breeding_pool;

			database::SQLCommand* sqlcmd_get_include_best_individual_in_breeding_pool;

			sqlcmd_get_include_best_individual_in_breeding_pool = new database::SQLCommand(&con, sqlstmt_get_include_best_individual_in_breeding_pool);

			sqlcmd_get_include_best_individual_in_breeding_pool->execute();

			if (sqlcmd_get_include_best_individual_in_breeding_pool->fetch_next())
				n = sqlcmd_get_include_best_individual_in_breeding_pool->get_field_as_long(1);

			delete sqlcmd_get_include_best_individual_in_breeding_pool;

			return n;
		}

		unsigned int load_example_cases()
		{
			unsigned int training_case_index = 0;
			unsigned int test_case_index = 0;

			std::string training_case_problem_str;
			std::string training_case_solution_str;
			std::string test_case_problem_str;
			std::string test_case_solution_str;

			std::deque<std::string> fitness_cases_problem;
			std::deque<std::string> fitness_cases_solution;

			database::SQLCommand* sqlcmd_get_example_cases;

			sqlcmd_get_example_cases = new database::SQLCommand(&con, sqlstmt_sqlcmd_load_example_cases);

			try
			{
				sqlcmd_get_example_cases->execute();

				if (sqlcmd_get_example_cases->is_result_set())
				{
					int fitness_case_count = 0;
					std::string fitness_case_problem_str;
					std::string fitness_case_solution_str;

					// Copy all fitness cases in the queue
					while (sqlcmd_get_example_cases->fetch_next())
					{
						// Get problem
						fitness_case_problem_str = sqlcmd_get_example_cases->get_field_as_string(1);
						fitness_cases_problem.push_front(fitness_case_problem_str);

						// Get solution
						fitness_case_solution_str = sqlcmd_get_example_cases->get_field_as_string(2);
						fitness_cases_solution.push_front(fitness_case_solution_str);

						fitness_case_count++;
					}
					while ((training_case_index < argmap::number_of_training_cases) && (fitness_case_count > 0))
					{
						CSVRow row;

						std::cout << "training case n = " << training_case_index << std::endl;

						// Get problem
						training_case_problem_str = fitness_cases_problem.back();
						fitness_cases_problem.pop_back();

						std::stringstream training_case_problem_stream;
						training_case_problem_stream.str(training_case_problem_str);

						while (training_case_problem_stream >> row)
						{
							for (int n = 0; n < row.size(); n++)
								training_cases_problem[training_case_index].push_back(std::stod(row[n]));
						}

						// Get solution
						training_case_solution_str = fitness_cases_solution.back();
						fitness_cases_solution.pop_back();
						std::stringstream training_case_solution_stream;
						training_case_solution_stream.str(training_case_solution_str);

						while (training_case_solution_stream >> row)
						{
							for (int n = 0; n < row.size(); n++)
								training_cases_solution[training_case_index].push_back(std::stod(row[n]));
						}

						training_case_index++;
						fitness_case_count--;
					}

					while ((test_case_index < argmap::number_of_test_cases) && (fitness_case_count > 0))
					{
						CSVRow row;

						std::cout << "test case n = " << test_case_index << std::endl;

						// Get problem
						test_case_problem_str = fitness_cases_problem.back();
						fitness_cases_problem.pop_back();
						std::stringstream test_case_problem_stream;
						test_case_problem_stream.str(test_case_problem_str);

						while (test_case_problem_stream >> row)
						{
							for (int n = 0; n < row.size(); n++)
								test_cases_problem[test_case_index].push_back(std::stod(row[n]));
						}

						// Get solution
						test_case_solution_str = fitness_cases_solution.back();
						fitness_cases_solution.pop_back();
						std::stringstream test_case_solution_stream;
						test_case_solution_stream.str(test_case_solution_str);

						while (test_case_solution_stream >> row)
						{
							for (int n = 0; n < row.size(); n++)
								test_cases_solution[test_case_index].push_back(std::stod(row[n]));
						}

						test_case_index++;
						fitness_case_count--;
					}
				}
			}
			catch (...)
			{
				delete sqlcmd_get_example_cases;

				return training_case_index + test_case_index;
			}

			delete sqlcmd_get_example_cases;

			return training_case_index + test_case_index;
		}

		unsigned int make_example_cases(int _example_cases_loaded)
		{
			unsigned int training_cases_created = 0;
			unsigned int test_cases_created = 0;

			if (_example_cases_loaded < argmap::number_of_training_cases)
			{
				for (int i = _example_cases_loaded; i < argmap::number_of_training_cases; i++)
				{
					int training_case_length = Utilities::random_integer(argmap::example_case_min_length, argmap::example_case_max_length);

					for (int j = 0; j < training_case_length; j++)
					{
						int n = Utilities::random_integer(argmap::example_case_upper_range);

						training_cases_problem[i].push_back(n);
						training_cases_solution[i].push_back(n);
					}

					std::sort(training_cases_solution[i].begin(), training_cases_solution[i].end());

					training_cases_created++;
				}
			}

			if ((_example_cases_loaded + training_cases_created) < argmap::number_of_training_cases + argmap::number_of_test_cases)
			{
				for (int i = _example_cases_loaded + training_cases_created - argmap::number_of_training_cases; i < argmap::number_of_test_cases; i++)
				{
					int test_case_length = Utilities::random_integer(argmap::example_case_min_length, argmap::example_case_max_length);

					for (int j = 0; j < test_case_length; j++)
					{
						int n = Utilities::random_integer(argmap::example_case_upper_range);

						test_cases_problem[i].push_back(n);
						test_cases_solution[i].push_back(n);
					}

					std::sort(test_cases_solution[i].begin(), test_cases_solution[i].end());

					test_cases_created++;
				}
			}

			return training_cases_created + test_cases_created;
		}

		void save_example_cases()
		{
			char buffer[33];

			database::SQLCommand* sqlcmd_insert_new_example_case;

			sqlcmd_insert_new_example_case = new database::SQLCommand(&con);

			// Begin a transaction
			sqlcmd_insert_new_example_case->begin_transaction();  

			// Delete previously saved test cases
			sqlcmd_insert_new_example_case->execute(sqlstmt_delete_all_example_cases);

			// Save new 
			sqlcmd_insert_new_example_case->set_command(sqlstmt_insert_new_example_case);

			for (int i = 0; i < argmap::number_of_training_cases; i++)
			{
				std::string training_case_input_str = "";
				std::string training_case_element = "";

				for (int training_case_data : training_cases_problem[i])
				{
					if (training_case_input_str.length() > 0)
						training_case_input_str += ",";

					_itoa_s(training_case_data, buffer, 10);
					training_case_input_str += buffer;
				}

				std::string training_case_output_str = "";
				training_case_element = "";

				for (int training_case_data : training_cases_solution[i])
				{
					if (training_case_output_str.length() > 0)
						training_case_output_str += ",";

					_itoa_s(training_case_data, buffer, 10);
					training_case_output_str += buffer;
				}

				sqlcmd_insert_new_example_case->set_as_string(1, training_case_input_str);
				sqlcmd_insert_new_example_case->set_as_string(2, training_case_output_str);
				sqlcmd_insert_new_example_case->execute();
			}

			for (int i = 0; i < argmap::number_of_test_cases; i++)
			{
				std::string test_case_input_str = "";
				std::string test_case_element = "";

				for (int test_case_data : test_cases_problem[i])
				{
					if (test_case_input_str.length() > 0)
						test_case_input_str += ",";

					_itoa_s(test_case_data, buffer, 10);
					test_case_input_str += buffer;
				}

				std::string test_case_output_str = "";
				test_case_element = "";

				for (int test_case_data : test_cases_solution[i])
				{
					if (test_case_output_str.length() > 0)
						test_case_output_str += ",";

					_itoa_s(test_case_data, buffer, 10);
					test_case_output_str += buffer;
				}

				sqlcmd_insert_new_example_case->set_as_string(1, test_case_input_str);
				sqlcmd_insert_new_example_case->set_as_string(2, test_case_output_str);
				sqlcmd_insert_new_example_case->execute();
			}

			// Commit transaction
			sqlcmd_insert_new_example_case->commit_transaction(); 

			delete sqlcmd_insert_new_example_case;
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
						if ((n % 1'000) == 0)
							std::cout << "n = " << n << std::endl;

						std::string genome = sqlcmd_get_individuals->get_field_as_string(2);
						pushGP::globals::population_agents[n].set_genome(genome);

						//pushGP::globals::population_agents[n].record_family_tree(
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(3)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(4)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(5)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(6)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(7)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(8)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(9)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(10)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(11)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(12)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(13)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(14)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(15)),
						//	StringToGuid(sqlcmd_get_individuals->get_field_as_string(16)));

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

		unsigned int make_pop_agents(Plush::Environment& _env, int _start)
		{
			unsigned int agents_created = 0;

			for (int n = _start; n < argmap::population_size; n++)
			{
//				pushGP::globals::population_agents[n].set_genome(pushGP::random_plush_genome());

				agents_created++;
			}

			// Cleanup thread factories
			_env.clear_stacks();

			return agents_created;
		}

		int run()
		{
			pushGP::SimulatedAnnealing sa;

			// Check if CPU is too hot and if so, wait for it to cool down.
			double temp = Utilities::GetCpuTemperature();

			std::cout << "CPU Temperature is " << temp << std::endl;

			// Check if there is enough memory to continue
			unsigned long percent_memory_use = Utilities::GetMemoryLoad();

			if (percent_memory_use > argmap::percent_memory_cap)
			{
				std::stringstream error;

				error << "Not enough free memory to continue.  Percent used = " << percent_memory_use;

				std::cerr << error.str() << std::endl;

				return 1;
			}

			// Setup
			Plush::Environment env;

			try
			{
				unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				unsigned int agents_created = 0;
				bool done = false;

				// Initialize database connection
				con.connect(argmap::db_init_datasource, argmap::db_init_catalog, argmap::db_user_id, argmap::db_user_password);

				// Load example cases.  Create more if not enough loaded.
				std::cout << "Load Example Cases" << std::endl;
				unsigned int example_cases_created = make_example_cases(load_example_cases());

				if (example_cases_created > 0)
					save_example_cases();

				// Load population.  Create more if not enough loaded.
				std::cout << "Create Population Agents" << std::endl;
				generation_number = get_last_saved_generation_number() + 1;
				agents_created = make_pop_agents(env, load_pop_agents());

				sa.set_cold();
				sa.set_tempareture(get_last_saved_temperature(sa.get_tempareture()));

				if (agents_created > 0)
					generation_number = 0;


				int best_individual = -1;
				double best_individual_score = std::numeric_limits<double>::max();
				double best_individual_error = get_last_best_individual_error(std::numeric_limits<double>::max());
				double prev_best_individual_error = get_last_prev_best_individual_error(std::numeric_limits<double>::max());
				int stalled_count = get_last_stalled_count(argmap::stalled_count_trigger);
				int cool_down_count = get_last_cool_down_count(argmap::cool_down_period);
				bool include_best_individual_in_breeding_pool = get_include_best_individual_in_breeding_pool(true);

			}
			catch (const std::exception& e)
			{
				env.clear_stacks();

				std::cerr << "Standard exception: " << e.what() << std::endl;
				throw;
			}
			catch (...)
			{
				env.clear_stacks();

				std::cerr << "Exception occurred" << std::endl;
				throw;
			}




			return 0;
		}
	}
}
