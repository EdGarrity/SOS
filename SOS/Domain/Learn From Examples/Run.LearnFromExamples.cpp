#define NOMINMAX

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
#include "../../Utilities/Conversion.h"
#include "ErrorFunction.LearnFromExample.h"
#include "../../PushGP/Breed.h"
#include "../../PushGP/AsyncBreed.h"
#include "../../PushGP/Random.h"
#include "..\..\Utilities\WorkOrderManager.h"
#include "..\..\Utilities\Debug.h"

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
			"           ,[BestIndividual_Training_Effort]"			// 22
			"           )"
			"     VALUES"
			"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
				//       1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2

		unsigned long get_last_saved_generation_number()
		{
			unsigned long n = 0;

			database::SQLCommand* sqlcmd_get_last_saved_generation_number;

			sqlcmd_get_last_saved_generation_number = new database::SQLCommand(&con, sqlstmt_get_last_saved_generation_number);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_saved_generation_number", "sqlcmd");
#endif

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

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_saved_temperature", "sqlcmd");
#endif

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

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_best_individual_error", "sqlcmd");
#endif
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

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_prev_best_individual_error", "sqlcmd");
#endif
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

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_stalled_count", "sqlcmd");
#endif
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

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_cool_down_count", "sqlcmd");
#endif
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

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_include_best_individual_in_breeding_pool", "sqlcmd");
#endif
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
#if DLEVEL > 0
				Utilities::debug_log(-1, "load_example_cases", "sqlcmd");
#endif
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

				std::stringstream error;
				error << "SQLConnection::load_example_cases()";
				std::cerr << error.str() << std::endl;

				return training_case_index + test_case_index;
			}

			delete sqlcmd_get_example_cases;

			return training_case_index + test_case_index;
		}

		unsigned int make_example_cases(int _example_cases_loaded)
		{
			unsigned int training_cases_created = 0;
			unsigned int test_cases_created = 0;

			//// Simple sorting test cases
			//if (_example_cases_loaded < argmap::number_of_training_cases)
			//{
			//	for (int i = _example_cases_loaded; i < argmap::number_of_training_cases; i++)
			//	{
			//		int training_case_length = Utilities::random_integer(argmap::example_case_min_length, argmap::example_case_max_length);

			//		for (int j = 0; j < training_case_length; j++)
			//		{
			//			int n = Utilities::random_integer(argmap::example_case_upper_range);

			//			training_cases_problem[i].push_back(n);
			//			training_cases_solution[i].push_back(n);
			//		}

			//		std::sort(training_cases_solution[i].begin(), training_cases_solution[i].end());

			//		training_cases_created++;
			//	}
			//}

			//if ((_example_cases_loaded + training_cases_created) < argmap::number_of_training_cases + argmap::number_of_test_cases)
			//{
			//	for (int i = _example_cases_loaded + training_cases_created - argmap::number_of_training_cases; i < argmap::number_of_test_cases; i++)
			//	{
			//		int test_case_length = Utilities::random_integer(argmap::example_case_min_length, argmap::example_case_max_length);

			//		for (int j = 0; j < test_case_length; j++)
			//		{
			//			int n = Utilities::random_integer(argmap::example_case_upper_range);

			//			test_cases_problem[i].push_back(n);
			//			test_cases_solution[i].push_back(n);
			//		}

			//		std::sort(test_cases_solution[i].begin(), test_cases_solution[i].end());

			//		test_cases_created++;
			//	}
			//}

			// Simple addition test cases using random length integers to add.
			if (_example_cases_loaded < argmap::number_of_training_cases)
			{
				for (int i = _example_cases_loaded; i < argmap::number_of_training_cases; i++)
				{
					int training_case_length = Utilities::random_integer(argmap::example_case_min_length, argmap::example_case_max_length);

					for (int j = 0; j < training_case_length; j++)
					{
						int n = Utilities::random_integer(argmap::example_case_upper_range);

						training_cases_problem[i].push_back(n);
					}

					int sum = 0;
					training_cases_solution[i].clear();

					for (int i : training_cases_problem[i])
						sum += i;

					training_cases_solution[i].push_back(sum);

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
					}

					int sum = 0;
					test_cases_solution[i].clear();

					for (int i : test_cases_problem[i])
						sum += i;

					test_cases_solution[i].push_back(sum);

					test_cases_created++;
				}
			}

			return training_cases_created + test_cases_created;
		}

		void save_example_cases()
		{
			char buffer[33] = "";

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

#if DLEVEL > 0
				Utilities::debug_log(-1, "save_example_cases", "sqlcmd");
#endif
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
#if DLEVEL > 0
				Utilities::debug_log(-1, "save_example_cases", "sqlcmd2");
#endif
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
#if DLEVEL > 0
				Utilities::debug_log(-1, "load_pop_agents", "sqlcmd");
#endif
				sqlcmd_get_individuals->execute();

				if (sqlcmd_get_individuals->is_result_set())
				{
					while ((sqlcmd_get_individuals->fetch_next()) && (n < argmap::population_size))
					{
						if ((n % 1'000) == 0)
							std::cout << "n = " << n << std::endl;

						std::string genome = Utilities::trim_copy(sqlcmd_get_individuals->get_field_as_string(2));

						if (genome.length() > 0)
						{
							pushGP::globals::population_agents[n].set_genome(genome);

							pushGP::globals::population_agents[n].record_family_tree(
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(3)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(4)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(5)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(6)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(7)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(8)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(9)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(10)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(11)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(12)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(13)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(14)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(15)),
								StringToGuid(sqlcmd_get_individuals->get_field_as_string(16)));

							n++;
						}

						else
							std::cout << "n = " << n << "  Ignoring empty genome string" << std::endl;
					}
				}
			}
			catch (...)
			{
				delete sqlcmd_get_individuals;

				std::stringstream error;
				error << "SQLConnection::load_pop_agents()";
				std::cerr << error.str() << std::endl;

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
				pushGP::make_random_plush_genome(pushGP::globals::population_agents[n].get_genome());
				agents_created++;
			}

			// Cleanup thread factories
			_env.clear_stacks();

			return agents_created;
		}

		void save_generation()
		{
			UUID NilUuid;

			// creates a nil-valued UUID
			RPC_STATUS ret=UuidCreateNil(&NilUuid);

			if (ret != RPC_S_OK)
				std::cout << "UuidCreateNil() did not return RPC_S_OK" << std::endl;

			database::SQLCommand* sqlcmd_delete_individuals;
			database::SQLCommand* sqlcmd_insert_new_individual;

			sqlcmd_delete_individuals = new database::SQLCommand(&con, sqlstmt_delete_individual);
			sqlcmd_insert_new_individual = new database::SQLCommand(&con);

			// Begin a transaction
			sqlcmd_insert_new_individual->begin_transaction();  //transaction->begin();

			// Delete previously saved generation
			sqlcmd_insert_new_individual->execute(sqlstmt_delete_individual);

			// Save new generation
			sqlcmd_insert_new_individual->set_command(sqlstmt_insert_new_individual);

			for (int n = 0; n < argmap::population_size; n++)
			{
				sqlcmd_insert_new_individual->set_as_integer(1, n + 1);
				sqlcmd_insert_new_individual->set_as_string(2, pushGP::globals::population_agents[n]);

				std::unordered_set<UUID> parents = pushGP::globals::population_agents[n].get_parents();
				auto it = parents.begin();

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(3, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(3, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(4, *it);
				else
					sqlcmd_insert_new_individual->set_as_GUID(4, NilUuid);

				parents = pushGP::globals::population_agents[n].get_grandparents();
				it = parents.begin();

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(5, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(5, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(6, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(6, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(7, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(7, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(8, *it);
				else
					sqlcmd_insert_new_individual->set_as_GUID(8, NilUuid);

				parents = pushGP::globals::population_agents[n].get_greatgrandparents();
				it = parents.begin();

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(9, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(9, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(10, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(10, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(11, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(11, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(12, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(12, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(13, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(13, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(14, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(14, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(15, *it++);
				else
					sqlcmd_insert_new_individual->set_as_GUID(15, NilUuid);

				if (it != parents.end())
					sqlcmd_insert_new_individual->set_as_GUID(16, *it);
				else
					sqlcmd_insert_new_individual->set_as_GUID(16, NilUuid);

				//Utilities::debug_log(-1, "save_generation", "sqlcmd");
				sqlcmd_insert_new_individual->execute();
			}

			// Commit transaction
			sqlcmd_insert_new_individual->commit_transaction();  //transaction->commit();

			delete sqlcmd_delete_individuals;
			delete sqlcmd_insert_new_individual;
		}

		std::tuple<int, double, double, int> compute_training_errors(Plush::Environment& _env,
			std::function<std::tuple<double, size_t>(Plush::Environment& _env,
				unsigned int _individual_index,
				std::vector<double>& _input_list,
				std::vector<double>& _output_list)> _run_individual_program,
			int _number_of_example_cases)
		{
			int individual_with_least_error = -1;
			int individual_with_best_score = -1;
			double min_error = (std::numeric_limits<double>::max)();
			double min_score = (std::numeric_limits<double>::max)();
			int max_effort_for_best_individual = 0;

			Plush::Environment* envp_local = new Plush::Environment;

			for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{
				int error_count_for_individual = 0;
				double avg_error_for_individual = 0.0;
				int max_effort_for_individual = 0;

				if ((individual_index % 100) == 0)
					std::cout << individual_index;

				for (int example_case = 0; example_case < _number_of_example_cases; example_case++)
				{
					std::vector<double> example_problem(training_cases_problem[example_case].begin(), training_cases_problem[example_case].end());
					std::vector<double> example_solution(training_cases_solution[example_case].begin(), training_cases_solution[example_case].end());

					// Run program
					auto [ error, effort ] = _run_individual_program(*envp_local, individual_index, example_problem, example_solution);

					if (error > 0.0)
						error_count_for_individual++;

					avg_error_for_individual += error;
					max_effort_for_individual = (max_effort_for_individual > effort) ? max_effort_for_individual : effort;

					//pushGP::globals::error_matrix[example_case][individual_index].store(error, std::memory_order_release);
					//pushGP::globals::error_matrix[example_case][individual_index] = error;
					//pushGP::globals::error_matrix.store(example_case, individual_index, error);
					//pushGP::globals::error_matrix[example_case][individual_index].store(error, std::memory_order_release);
					pushGP::globals::error_matrix.store(-1, example_case, individual_index, error);
					pushGP::globals::effort_matrix.store(-1, example_case, individual_index, effort);
				}

				// Calculate the average error for all example cases
				avg_error_for_individual /= (double)_number_of_example_cases;

				double score = (double)error_count_for_individual / (double)_number_of_example_cases;

				if ((score < 1.0) && (score < min_score))
				{
					min_score = score;
					individual_with_best_score = individual_index;
					max_effort_for_best_individual = max_effort_for_individual;
				}

				if (avg_error_for_individual < min_error)
				{
					min_error = avg_error_for_individual;
					individual_with_least_error = individual_index;
					max_effort_for_best_individual = max_effort_for_individual;
				}

				if ((individual_index % 100) == 0)
					std::cout << std::endl;
			}

			delete envp_local;

			std::cout << std::endl;

			return std::make_tuple
			(
				(individual_with_best_score == -1) ? individual_with_least_error : individual_with_best_score,
				min_score,
				min_error,
				max_effort_for_best_individual
			);
		}

		std::tuple<int, double, double, int> parallel_compute_training_errors(Plush::Environment& _env,
			std::function<std::tuple<double, size_t>(Plush::Environment& _env,
				unsigned int _individual_index,
				std::vector<double>& _input_list,
				std::vector<double>& _output_list)> _run_individual_program,
			int _number_of_example_cases)
		{
			int individual_with_least_error = -1;
			int individual_with_best_score = -1;
			double min_error = (std::numeric_limits<double>::max)();
			double min_score = (std::numeric_limits<double>::max)();
			const unsigned int zero = 0;
			int max_effort_for_best_individual = 0;

			parallel_for(zero, domain::argmap::population_size / domain::argmap::thread_chunk_size, [&, _number_of_example_cases](const unsigned int chunk_index)
			{
				for (int individual_index = chunk_index * domain::argmap::thread_chunk_size; individual_index < (chunk_index + 1) * domain::argmap::thread_chunk_size; individual_index++)
				{
					int error_count_for_individual = 0;
					double avg_error_for_individual = 0.0;
					int max_effort_for_individual = 0;

					for (int example_case = 0; example_case < _number_of_example_cases; example_case++)
					{
						std::vector<double> example_problem(training_cases_problem[example_case].begin(), training_cases_problem[example_case].end());
						std::vector<double> example_solution(training_cases_solution[example_case].begin(), training_cases_solution[example_case].end());

						// Run program
						auto [ error, effort ] = run_individual_threadsafe(_env, individual_index, example_problem, example_solution);

						if (error > 0.0)
							error_count_for_individual++;

						avg_error_for_individual += error;
						max_effort_for_individual = (max_effort_for_individual > effort) ? max_effort_for_individual : effort;

						//pushGP::globals::error_matrix[example_case][individual_index].store(error, std::memory_order_release);
						//pushGP::globals::error_matrix[example_case][individual_index] = error;
						//pushGP::globals::error_matrix.store(example_case, individual_index, error);
						//pushGP::globals::error_matrix[example_case][individual_index].store(error, std::memory_order_release);
						pushGP::globals::error_matrix.store(-1, example_case, individual_index, error);
						pushGP::globals::effort_matrix.store(-1, example_case, individual_index, effort);
					}

					// Calculate the average error for all example cases
					avg_error_for_individual /= (double)_number_of_example_cases;

					double score = (double)error_count_for_individual / (double)_number_of_example_cases;

					if ((score < 1.0) && (score < min_score))
					{
						min_score = score;
						individual_with_best_score = individual_index;
						max_effort_for_best_individual = max_effort_for_individual;
					}

					if (avg_error_for_individual < min_error)
					{
						min_error = avg_error_for_individual;
						individual_with_least_error = individual_index;
						max_effort_for_best_individual = max_effort_for_individual;
					}
				}
			});

			return std::make_tuple
			(
				(individual_with_best_score == -1) ? individual_with_least_error : individual_with_best_score,
				min_score,
				min_error,
				max_effort_for_best_individual
			);
		}

		std::tuple<int, double, double, int> compute_training_errors_thread_safe(Plush::Environment& _env,
			std::function<std::tuple<double, size_t>(Plush::Environment& _env,
				unsigned int _individual_index,
				std::vector<double>& _input_list,
				std::vector<double>& _output_list)> _run_individual_program,
			int _number_of_example_cases)
		{
			int individual_with_least_error = -1;
			int individual_with_best_score = -1;
			double min_error = (std::numeric_limits<double>::max)();
			double min_score = (std::numeric_limits<double>::max)();
			int max_effort_for_best_individual = 0;

			std::cout << "compute_training_errors_thread_safe() - Process threads" << std::endl;

			Utilities::work_order_manager.stop();

			for (int example_case = 0; example_case < _number_of_example_cases; example_case++)
			{
				for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
				{
					std::vector<double> example_problem(training_cases_problem[example_case].begin(), training_cases_problem[example_case].end());
					std::vector<double> example_solution(training_cases_solution[example_case].begin(), training_cases_solution[example_case].end());

					Utilities::work_order_manager.push(individual_index, example_case, example_problem, example_solution);
				}
			}

			Utilities::work_order_manager.start();
			Utilities::work_order_manager.wait_for_all_threads_to_complete();

			std::cout << "compute_training_errors_thread_safe() - Aggregate errors" << std::endl;

			for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{
				int error_count_for_individual = 0;
				double avg_error_for_individual = 0.0;
				int max_effort_for_individual = 0;

				for (int example_case = 0; example_case < _number_of_example_cases; example_case++)
				{
					//double error = pushGP::globals::error_matrix[example_case][individual_index].load(std::memory_order_acquire);
					//double error = pushGP::globals::error_matrix[example_case][individual_index];
					//double error = pushGP::globals::error_matrix.load(example_case, individual_index);
					//double error = pushGP::globals::error_matrix[example_case][individual_index].load(std::memory_order_acquire);
					double error = pushGP::globals::error_matrix.load(example_case, individual_index);
					int effort = pushGP::globals::effort_matrix.load(example_case, individual_index);

					if (error > 0.0)
						error_count_for_individual++;

					avg_error_for_individual += error;
					max_effort_for_individual = (max_effort_for_individual > effort) ? max_effort_for_individual : effort;
				}

				// Calculate the average error for all example cases
				avg_error_for_individual /= (double)_number_of_example_cases;

				double score = (double)error_count_for_individual / (double)_number_of_example_cases;

				if ((score < 1.0) && (score < min_score))
				{
					min_score = score;
					individual_with_best_score = individual_index;
					max_effort_for_best_individual = max_effort_for_individual;
				}

				if (avg_error_for_individual < min_error)
				{
					min_error = avg_error_for_individual;
					individual_with_least_error = individual_index;
					max_effort_for_best_individual = max_effort_for_individual;
				}
			}

			std::cout << "compute_training_errors_thread_safe() - Return result" << std::endl;

			return std::make_tuple
			(
				(individual_with_best_score == -1) ? individual_with_least_error : individual_with_best_score,
				min_score,
				min_error,
				max_effort_for_best_individual
			);
		}

		double compute_test_errors(Plush::Environment& _env,
			std::function<std::tuple<double, size_t>(Plush::Environment& _env,
				unsigned int _individual_index,
				std::vector<double>& _example_problem,
				std::vector<double>& _example_solution)> _run_individual_program,
			int _individual_index)
		{
			double error = (std::numeric_limits<double>::max)();
			int error_count = 0;

			for (int example_case = 0; example_case < argmap::number_of_test_cases; ++example_case)
			{
				std::cout << ".";

				std::vector<double> example_problem(test_cases_problem[example_case].begin(), test_cases_problem[example_case].end());
				std::vector<double> example_solution(test_cases_solution[example_case].begin(), test_cases_solution[example_case].end());

				auto [ example_case_error, effort ] = _run_individual_program(_env, _individual_index, example_problem, example_solution);

				if (example_case_error > 0.0)
					error_count++;
			}

			error = (double)error_count / (double)argmap::number_of_test_cases;

			std::cout << std::endl;
			std::cout << std::endl;

			return error;
		}

		void produce_new_offspring(int _number_of_example_cases,
			unsigned int _best_individual,
			pushGP::SimulatedAnnealing& sa,
			bool _include_best_individual_in_breeding_pool)
		{
			try
			{
				std::set<std::string> set_of_gnomes;
				combinable<pushGP::globals::Training_case_min_error_type> training_case_min_error;

				// Reset children.
				std::cout << "  Reset children" << std::endl;
				for (unsigned int n = 0; n < argmap::population_size; n++)
					pushGP::globals::child_agents[n].clear_genome();

				// Breed new generation
				std::cout << "  Breed new generation" << std::endl;

				for (unsigned int individual_index = 0; individual_index < argmap::population_size; individual_index++)
				{
					Plush::Genome<Plush::CodeAtom>& child_genome = pushGP::globals::child_agents[individual_index].get_genome();

					// Keep the best individual
	//				std::cout << "  Keep the best individual" << std::endl;
					if ((!_include_best_individual_in_breeding_pool) && (individual_index == _best_individual))
						pushGP::globals::child_agents[individual_index].copy(pushGP::globals::population_agents[individual_index]);

					else
					{
						//					std::cout << "  breed(" << individual_index << ")" << std::endl;
						if (individual_index % 100 == 0)
							std::cout << "B";

						pushGP::breed(individual_index,
							_number_of_example_cases,
							training_case_min_error,
							sa,
							_include_best_individual_in_breeding_pool,
							_best_individual);

						// If a child with the same genome already exists, create a new random child.
	//					std::cout << "  If a child with the same genome already exists, create a new random child." << std::endl;
						if (set_of_gnomes.insert(pushGP::globals::child_agents[individual_index].get_genome_as_string()).second == false)
						{
							//						std::cout << "  create a new random child." << std::endl;
							//						pushGP::globals::child_agents[individual_index].set_genome(pushGP::make_random_plush_genome(genome));
							pushGP::make_random_plush_genome(child_genome);
						}
					}
				}

				std::cout << std::endl;

				// Keep the best individuals for each test case
	//			std::cout << "  Keep the best individuals for each test case" << std::endl;
				std::cout << ".";
				if (!_include_best_individual_in_breeding_pool)
				{
					for (unsigned int training_case = 0; training_case < domain::argmap::number_of_training_cases; training_case++)
					{
						unsigned int best_individual_for_training_case = training_case_min_error.local().individual_with_minimum_error_for_training_case[training_case];

						if (best_individual_for_training_case < (std::numeric_limits<unsigned int>::max)())
						{
							//						std::cout << "  						pushGP::globals::child_agents[best_individual_for_training_case].copy(pushGP::globals::population_agents[best_individual_for_training_case]);" << std::endl;
							pushGP::globals::child_agents[best_individual_for_training_case].copy(pushGP::globals::population_agents[best_individual_for_training_case]);
						}
					}
				}

				std::cout << std::endl;
			}
			catch (const std::exception& e)
			{
				std::stringstream error;

				error << "Standard exception: " << e.what();

				std::cerr << error.str() << std::endl;

				throw;
			}
			catch (...)
			{
				std::stringstream error;

				error << "Exception occurred";

				std::cerr << error.str() << std::endl;

				throw;
			}
		}

		void parallel_produce_new_offspring(int _number_of_example_cases,
			unsigned int _best_individual,
			pushGP::SimulatedAnnealing& sa,
			bool _include_best_individual_in_breeding_pool)
		{
			concurrent_unordered_set<std::string> set_of_gnomes;
			combinable<pushGP::globals::Training_case_min_error_type> training_case_min_error_sub_computations;
			pushGP::globals::Training_case_min_error_type training_case_min_error;

			// Reset children.
			std::cout << "  Reset children" << std::endl;
			for (unsigned int n = 0; n < argmap::population_size; n++)
				pushGP::globals::child_agents[n].clear_genome();

			// Breed new generation
			std::cout << "  Breed new generation" << std::endl;

			const unsigned int zero = 0;
			parallel_for(zero, domain::argmap::population_size, [&, _best_individual, _number_of_example_cases](const unsigned int individual_index)
			{
				// Keep the best individual
				if (individual_index == _best_individual)
					pushGP::globals::child_agents[individual_index].copy(pushGP::globals::population_agents[individual_index]);

				else
				{
					pushGP::breed(individual_index,
						_number_of_example_cases,
						training_case_min_error_sub_computations,
						sa,
						_include_best_individual_in_breeding_pool,
						_best_individual);
				}
			});

			parallel_for(zero, domain::argmap::population_size, [&, _best_individual, _number_of_example_cases](const unsigned int individual_index)
			{
				// If a child with the same genome already exists, create a new random child.
				if (individual_index != _best_individual)
				{
					Plush::Genome<Plush::CodeAtom>& child_genome = pushGP::globals::child_agents[individual_index].get_genome();

					if (set_of_gnomes.insert(pushGP::globals::child_agents[individual_index].get_genome_as_string()).second == false)
						pushGP::make_random_plush_genome(child_genome);
				}
			});

			// Combine the Training Case Minimum Errors
			training_case_min_error_sub_computations.combine_each([&](pushGP::globals::Training_case_min_error_type &local)
			{
				for (unsigned int example_case = 0; example_case < domain::argmap::number_of_training_cases; example_case++)
				{
					if (training_case_min_error.minimum_error_array_by_example_case[example_case] > local.minimum_error_array_by_example_case[example_case])
					{
						training_case_min_error.minimum_error_array_by_example_case[example_case] = local.minimum_error_array_by_example_case[example_case];
						training_case_min_error.individual_with_minimum_error_for_training_case[example_case] = local.individual_with_minimum_error_for_training_case[example_case];
					}
				}
			});

			// Keep the best individuals for each test case
			if (_include_best_individual_in_breeding_pool)
			{
				for (unsigned int training_case = 0; training_case < domain::argmap::number_of_training_cases; training_case++)
				{
					unsigned int best_individual_for_training_case = training_case_min_error.individual_with_minimum_error_for_training_case[training_case];

					if (best_individual_for_training_case < (std::numeric_limits<unsigned int>::max)())
						pushGP::globals::child_agents[best_individual_for_training_case].copy(pushGP::globals::population_agents[best_individual_for_training_case]);
				}
			}

			std::cout << std::endl;
		}

		void install_next_generation()
		{
			for (unsigned int n = 0; n < argmap::population_size; n++)
				pushGP::globals::population_agents[n].copy(pushGP::globals::child_agents[n]);
		}

		void generate_status_report(unsigned int _generation_number,
			unsigned int _generations_completed_this_session,
			unsigned int _best_individual_id,
			double _best_individual_training_score,
			unsigned int _best_individual_training_effort,
			double _best_individual_training_error,
			double _best_individual_prev_training_error,
			double _average_traiing_error,
			double _standard_deviation,
			double _best_individual_test_score,
			double _temperature,
			unsigned long _stalled_count,
			unsigned int _cool_down_count,
			bool _include_best_individual_in_breeding_pool,
			std::string _best_gnome)
		{
			database::SQLCommand* sqlcmd_save_status_report;

			// Normalize data
			if (_average_traiing_error > 1000.0)
				_average_traiing_error = 1000.0;

			if (_standard_deviation > 1000.0)
				_standard_deviation = 1000.0;

			sqlcmd_save_status_report = new database::SQLCommand(&con, sqlstmt_save_status_report);

			sqlcmd_save_status_report->set_as_integer(1, _generation_number);
			sqlcmd_save_status_report->set_as_integer(2, _generations_completed_this_session);
			sqlcmd_save_status_report->set_as_integer(3, _best_individual_id);
			sqlcmd_save_status_report->set_as_float(4, _best_individual_training_score);
			sqlcmd_save_status_report->set_as_float(5, _best_individual_training_error);
			sqlcmd_save_status_report->set_as_float(6, _average_traiing_error);
			sqlcmd_save_status_report->set_as_float(7, _standard_deviation);
			sqlcmd_save_status_report->set_as_float(8, _best_individual_test_score);
			sqlcmd_save_status_report->set_as_integer(9, argmap::number_of_training_cases);
			sqlcmd_save_status_report->set_as_integer(10, argmap::number_of_test_cases);
			sqlcmd_save_status_report->set_as_string(11, _best_gnome);
			sqlcmd_save_status_report->set_as_integer(12, argmap::population_size);
			sqlcmd_save_status_report->set_as_float(13, argmap::alternation_rate);
			sqlcmd_save_status_report->set_as_float(14, argmap::uniform_mutation_rate);
			sqlcmd_save_status_report->set_as_integer(15, argmap::example_case_max_length);
			sqlcmd_save_status_report->set_as_integer(16, argmap::example_case_upper_range);
			sqlcmd_save_status_report->set_as_float(17, _temperature);
			sqlcmd_save_status_report->set_as_float(18, _best_individual_prev_training_error);
			sqlcmd_save_status_report->set_as_integer(19, _stalled_count);
			sqlcmd_save_status_report->set_as_integer(20, _cool_down_count);
			sqlcmd_save_status_report->set_as_integer(21, _include_best_individual_in_breeding_pool);
			sqlcmd_save_status_report->set_as_integer(22, _best_individual_training_effort);

#if DLEVEL > 0
			Utilities::debug_log(-1, "generate_status_report", "sqlcmd");
			Utilities::debug_log(-1, "generate_status_report", "_generation_number=" + std::to_string(_generation_number));
			Utilities::debug_log(-1, "generate_status_report", "_generations_completed_this_session=" + std::to_string(_generations_completed_this_session));
			Utilities::debug_log(-1, "generate_status_report", "_best_individual_id=" + std::to_string(_best_individual_id));
			Utilities::debug_log(-1, "generate_status_report", "_best_individual_training_score=" + std::to_string(_best_individual_training_score));
			Utilities::debug_log(-1, "generate_status_report", "_best_individual_training_error=" + std::to_string(_best_individual_training_error));
			Utilities::debug_log(-1, "generate_status_report", "_average_traiing_error=" + std::to_string(_average_traiing_error));
			Utilities::debug_log(-1, "generate_status_report", "_standard_deviation=" + std::to_string(_standard_deviation));
			Utilities::debug_log(-1, "generate_status_report", "_best_individual_test_score=" + std::to_string(_best_individual_test_score));
			Utilities::debug_log(-1, "generate_status_report", "argmap::number_of_training_cases=" + std::to_string(argmap::number_of_training_cases));
			Utilities::debug_log(-1, "generate_status_report", "argmap::number_of_test_cases=" + std::to_string(argmap::number_of_test_cases));
			Utilities::debug_log(-1, "generate_status_report", "_best_gnome=" + _best_gnome);
			Utilities::debug_log(-1, "generate_status_report", "argmap::population_size=" + std::to_string(argmap::population_size));
			Utilities::debug_log(-1, "generate_status_report", "argmap::alternation_rate=" + std::to_string(argmap::alternation_rate));
			Utilities::debug_log(-1, "generate_status_report", "argmap::uniform_mutation_rate=" + std::to_string(argmap::uniform_mutation_rate));
			Utilities::debug_log(-1, "generate_status_report", "argmap::example_case_max_length=" + std::to_string(argmap::example_case_max_length));
			Utilities::debug_log(-1, "generate_status_report", "argmap::example_case_upper_range=" + std::to_string(argmap::example_case_upper_range));
			Utilities::debug_log(-1, "generate_status_report", "_temperature=" + std::to_string(_temperature));
			Utilities::debug_log(-1, "generate_status_report", "_best_individual_prev_training_error=" + std::to_string(_best_individual_prev_training_error));
			Utilities::debug_log(-1, "generate_status_report", "_stalled_count=" + std::to_string(_stalled_count));
			Utilities::debug_log(-1, "generate_status_report", "_cool_down_count=" + std::to_string(_cool_down_count));
			Utilities::debug_log(-1, "generate_status_report", "_include_best_individual_in_breeding_pool=" + std::to_string(_include_best_individual_in_breeding_pool));
#endif
			sqlcmd_save_status_report->execute();

			delete sqlcmd_save_status_report;

#if DLEVEL > 0
			Utilities::debug_log(-1, "generate_status_report", "Exit.");
#endif
		}

		// May need to move to thread local.
		Plush::Environment env;

		int run()
		{
			std::string debug_message;

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

#if DLEVEL > 0
				debug_message = error.str();
				Utilities::debug_log(-1, "run", debug_message);
#endif
				return 1;
			}

			// Setup
			// moved to global.  WIll need to be moved to thread global for parallel execution
//			Plush::Environment env;

			try
			{
				unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				unsigned int agents_created = 0;
				bool done = false;

				// Allocate meneory for Individuals (See https://stackoverflow.com/questions/19803162/array-size-error-x64-process)
				size_t sz = domain::argmap::population_size;
				pushGP::globals::population_agents = new pushGP::Individual[sz];
				pushGP::globals::child_agents = new pushGP::Individual[sz];

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

				while ((!done) && (generations_completed_this_session < argmap::max_generations_in_one_session))
				{
					if ((std::fabs(best_individual_error - prev_best_individual_error) < argmap::stalled_delta) && (cool_down_count <= 0))
						stalled_count = (stalled_count < 0) ? 0 : stalled_count - 1;

					else
						stalled_count = argmap::stalled_count_trigger;

					if (stalled_count <= 0)
					{
						sa.set_hot();
						cool_down_count = argmap::cool_down_period;
						include_best_individual_in_breeding_pool = false;

						std::cout << "Heat up " << sa.get_tempareture() << std::endl;
					}
					else
					{
						sa.cool_down();
						cool_down_count = (cool_down_count < 0) ? 0 : cool_down_count - 1;

						std::cout << "Cool down " << sa.get_tempareture() << std::endl;
					}

					prev_best_individual_error = best_individual_error;

					// Check if CPU is too hot and if so, wait for it to cool down.
					double temp = Utilities::GetCpuTemperature();

					std::cout << "CPU Temperature: Min = " << argmap::cool_temperature << " Max = " << argmap::hot_temperature << " Current Temp = " << temp << std::endl;

					if (temp > argmap::hot_temperature)
					{
						std::cout << "CPU is too hot.  Waiting for it to cool down." << std::endl;

						do
						{
							std::this_thread::sleep_for(std::chrono::minutes(argmap::cool_down_minutes));
							temp = Utilities::GetCpuTemperature();

							std::cout << "CPU Temperature is " << temp << std::endl;
						} while (temp > argmap::cool_temperature);

						std::cout << "CPU is now cool enough to continue." << std::endl;
						std::cout << std::endl;
					}

					// Check if there is enough memory to continue
					unsigned long percent_memory_use = Utilities::GetMemoryLoad();

					//debug_message = ",Percent_Free_Memory=" + std::to_string(percent_memory_use);
					//debug_message += ",Generation=" + std::to_string(generation_number);
					//debug_message += ",Session=" + std::to_string(generations_completed_this_session);
					//Utilities::debug_log(-1, "run", debug_message);

					if (percent_memory_use > argmap::percent_memory_cap)
					{
						std::stringstream error;

						error << "Not enough free memory to continue.  Percent used = " << percent_memory_use;

						std::cerr << error.str() << std::endl;

#if DLEVEL > 0
						debug_message = error.str();
						debug_message += ",Generation=" + std::to_string(generation_number);
						debug_message += ",Session=" + std::to_string(generations_completed_this_session);
						Utilities::debug_log(-1, "run", debug_message);
#endif
						return 1;
					}

					// Reset variables which track the minimum error for this test case and the individual who achived the minimum error 
					std::cout << "Reset variables which track the minimum error for this test case and the individual who achived the minimum error " << std::endl;

					for (int ind = 0; ind < argmap::population_size; ind++)
					{
						for (int training_case_index = 0; training_case_index < argmap::number_of_training_cases; training_case_index++)
						{
							pushGP::globals::error_matrix.store(-1, training_case_index, ind, 0);
							pushGP::globals::effort_matrix.store(-1, training_case_index, ind, 0);
						}
					}

					std::cout << "Generation " << generation_number << std::endl;
					std::cout << "Session " << generations_completed_this_session << std::endl;

#if DLEVEL > 0
					debug_message = "Reset variables which track the minimum error for this test case and the individual who achived the minimum error";
					debug_message += ",Generation=" + std::to_string(generation_number);
					debug_message += ",Session=" + std::to_string(generations_completed_this_session);
					Utilities::debug_log(-1, "run", debug_message);
#endif
					save_generation();

					std::cout << "Run Programs with Training Cases" << std::endl;
#if DLEVEL > 0
					Utilities::debug_log(-1, "run", "Run Programs with Training Cases");
#endif
					std::tuple<int, double, double, int> best_individual_score_error;

					if (argmap::use_multithreading)
						best_individual_score_error = compute_training_errors_thread_safe(
							env, 
							run_individual_threadsafe,
							argmap::number_of_training_cases);
					else if (argmap::use_PPL)
						best_individual_score_error = parallel_compute_training_errors(
							env,
							run_individual_threadsafe,
							argmap::number_of_training_cases);
					else
						best_individual_score_error = compute_training_errors(env, run_individual_threadsafe, argmap::number_of_training_cases);

					best_individual = std::get<0>(best_individual_score_error);
					best_individual_score = std::get<1>(best_individual_score_error);
					best_individual_error = std::get<2>(best_individual_score_error);

					std::cout << "Produce New Offspring" << std::endl;
#if DLEVEL > 0
					Utilities::debug_log(-1, "run", "Produce New Offspring");
#endif

#if TRACE_LEVEL>0

					Utilities::flush_trace_queue();
#endif

					if (argmap::use_PPL)
						parallel_produce_new_offspring(argmap::number_of_training_cases,
							best_individual,
							sa,
							include_best_individual_in_breeding_pool);

					else
						produce_new_offspring(argmap::number_of_training_cases,
							best_individual,
							sa,
							include_best_individual_in_breeding_pool);

					std::cout << "Run Best Individual's Program with Test Cases" << std::endl;

					std::string genome = pushGP::globals::population_agents[best_individual].get_genome_as_string();

					std::cout << "best_individual = " << best_individual << std::endl;
					std::cout << "genome = " << genome << std::endl;

#if DLEVEL > 0
					debug_message = "Run Best Individual's Program with Test Cases";
					debug_message += ",best_individual=" + std::to_string(best_individual);
					debug_message += ",genome=" + genome;
					Utilities::debug_log(-1, "run", debug_message);
#endif
					double test_case_score = compute_test_errors(env, run_individual_threadsafe, best_individual);

					std::cout << "test_case_error = " << test_case_score << std::endl;
					std::cout << std::endl;

#if DLEVEL > 0
					debug_message = "compute_test_errors";
					debug_message += ",test_case_error=" + std::to_string(test_case_score);
					Utilities::debug_log(-1, "run", debug_message);
#endif
					std::cout << "Generate Status Report" << std::endl;

#if DLEVEL > 0
					Utilities::debug_log(-1, "run", "Generate Status Report");
#endif
					double average_traiing_error = 0.0;
					int traiing_effort = 0;

					for (int ind = 0; ind < argmap::population_size; ind++)
					{
						for (int training_case_index = 0; training_case_index < argmap::number_of_training_cases; training_case_index++)
						{
#if DLEVEL > 0
							debug_message = "error_matrix";
							debug_message += ",training_case_index=" + std::to_string(training_case_index);
							debug_message += ",ind=" + std::to_string(ind);
							//debug_message += ",error=" + std::to_string(pushGP::globals::error_matrix[training_case_index][ind].load(std::memory_order_acquire));
							//debug_message += ",error=" + std::to_string(pushGP::globals::error_matrix[training_case_index][ind]);
							//debug_message += ",error=" + std::to_string(pushGP::globals::error_matrix.load(training_case_index, ind));
							//debug_message += ",error=" + std::to_string(pushGP::globals::error_matrix[training_case_index][ind].load(std::memory_order_acquire));
							debug_message += ",error=" + std::to_string(pushGP::globals::error_matrix.load(training_case_index, ind));

							Utilities::debug_log(-1, "run", debug_message);
#endif
							//average_traiing_error += pushGP::globals::error_matrix[training_case_index][ind].load(std::memory_order_acquire);
							//average_traiing_error += pushGP::globals::error_matrix[training_case_index][ind];
							//average_traiing_error += pushGP::globals::error_matrix.load(training_case_index, ind);
							//average_traiing_error += pushGP::globals::error_matrix[training_case_index][ind].load(std::memory_order_acquire);
							average_traiing_error += pushGP::globals::error_matrix.load(training_case_index, ind);
							traiing_effort += pushGP::globals::effort_matrix.load(training_case_index, ind);
						}
					}
					average_traiing_error /= (double)(domain::argmap::population_size * argmap::number_of_training_cases);

					double standard_deviation = 0.0;
					for (int ind = 0; ind < argmap::population_size; ind++)
					{
						for (int training_case_index = 0; training_case_index < argmap::number_of_training_cases; training_case_index++)
						{
							//double error = pushGP::globals::error_matrix[training_case_index][ind].load(std::memory_order_acquire);
							//double error = pushGP::globals::error_matrix[training_case_index][ind];
							//double error = pushGP::globals::error_matrix.load(training_case_index, ind);
							//double error = pushGP::globals::error_matrix[training_case_index][ind].load(std::memory_order_acquire);
							double error = pushGP::globals::error_matrix.load(training_case_index, ind);

							standard_deviation += (error - average_traiing_error) * (error - average_traiing_error);
						}
					}
					standard_deviation /= (double)(domain::argmap::population_size * argmap::number_of_training_cases);
					standard_deviation = std::sqrt(standard_deviation);

					generate_status_report(generation_number,
						generations_completed_this_session,
						best_individual,
						best_individual_score,
						traiing_effort,
						best_individual_error,
						prev_best_individual_error,
						average_traiing_error,
						standard_deviation,
						test_case_score,
						sa.get_tempareture(),
						stalled_count,
						cool_down_count,
						include_best_individual_in_breeding_pool,
						pushGP::globals::population_agents[best_individual]
					);

					std::cout << "Install New Generation" << std::endl;

#if DLEVEL > 0
					Utilities::debug_log(-1, "run", "Install New Generation");
#endif
					install_next_generation();
					generation_number++;
					generations_completed_this_session++;
				}

				delete[] pushGP::globals::population_agents;
				delete[] pushGP::globals::child_agents;
			}
			catch (const std::exception& e)
			{
				delete[] pushGP::globals::population_agents;
				delete[] pushGP::globals::child_agents;

#if DLEVEL > 0
				std::string err_msg = "Standard exception: ";
				err_msg += e.what();
				Utilities::debug_log(-1, "run", err_msg);
#endif
				env.clear_stacks();

//				std::cerr << "Standard exception: " << e.what() << std::endl;
				std::stringstream error;

				error << "Standard exception: " << e.what();

				std::cerr << error.str() << std::endl;

#if DLEVEL > 0
				debug_message = error.str();
				Utilities::debug_log(-1, "run", debug_message);
#endif
				throw;
			}
			catch (...)
			{
				delete[] pushGP::globals::population_agents;
				delete[] pushGP::globals::child_agents;

#if DLEVEL > 0
				Utilities::debug_log(-1, "run", "Exception occurred");
#endif
				env.clear_stacks();

//				std::cerr << "Exception occurred" << std::endl;
				std::stringstream error;

				error << "Exception occurred";

				std::cerr << error.str() << std::endl;

#if DLEVEL > 0
				debug_message = error.str();
				Utilities::debug_log(-1, "run", debug_message);
#endif
				throw;
			}

			return 0;
		}
	}
}
