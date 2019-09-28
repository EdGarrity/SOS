#include "Run.LearnFromExamples.h"
#include "ErrorFunction.LearnFromExample.h"
#include "../../Database/SQLCommand.h"
#include "../../Database/SQLField.h"
#include "../../PushGP/Globals.h"
#include "../../PushGP/Random.h"
#include "../../PushGP/Breed.h"
#include "../../PushP/ExecInstruction.h"
#include "../../Utilities/CSVIterator.h"
#include "../../PushGP/Utilities.h"
#include "../../Plush/Genome.h"
#include "../../Utilities/Conversion.h"
#include "../../PushGP/AsyncBreed.h"
#include <functional>
#include <limits>
#include <sstream>
#include <string>
#include <deque>

namespace domain
{
	namespace learn_from_examples
	{
		double score_array[argmap::population_size];

		std::vector<double> training_cases_problem[argmap::number_of_training_cases];
		std::vector<double> training_cases_solution[argmap::number_of_training_cases];
		std::vector<double> test_cases_problem[argmap::number_of_test_cases];
		std::vector<double> test_cases_solution[argmap::number_of_test_cases];
		std::deque<std::string> fitness_cases_problem;
		std::deque<std::string> fitness_cases_solution;

		database::SQLConnection con;

		const std::string sqlstmt_get_last_saved_generation_number = "SELECT TOP 1 [Generation] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
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
		
//		const std::string sqlstmt_insert_new_individual("INSERT INTO [dbo].[Individuals] ([Individual_ID], [Genome]) VALUES (?, ?);");

		const std::string sqlstmt_save_status_report("INSERT INTO [dbo].[ProgressLog]"
			"           ("
			"            [Generation]"								// 1
			"           ,[Generations_Completed_This_Session]"		// 2
			"           ,[BestIndividual_ID]"						// 3
			"           ,[BestIndividual_Training_Score]"			// 4
			"           ,[BestIndividual_Training_Error]"			// 5
			"           ,[Average_Training_Error]"					// 6
			"           ,[BestIndividual_Test_Score]"				// 7
			"           ,[Number_Of_Training_Cases]"				// 8
			"           ,[Number_Of_Test_Cases]"					// 9
			"           ,[Best_Genome]"								// 10
			"           ,[Population_Size]"							// 11
			"           ,[Alternation_Rate]"						// 12
			"           ,[Uniform_Mutation_Rate]"					// 13
			"           ,[Example_Case_Max_Length]"					// 14
			"           ,[Example_Case_Upper_Range]"				// 15
			"           )"
			"     VALUES"
			"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		        //       1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

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

		unsigned int load_example_cases()
		{
			unsigned int training_case_index = 0;
			unsigned int test_case_index = 0;

			std::string training_case_problem_str;
			std::string training_case_solution_str;
			std::string test_case_problem_str;
			std::string test_case_solution_str;

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
						std::vector<double> training_case_problem;
						std::vector<double> training_case_solution;
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
								training_case_problem.push_back(std::stod(row[n]));
						}

						training_cases_problem[training_case_index] = training_case_problem;

						// Get solution
						training_case_solution_str = fitness_cases_solution.back();
						fitness_cases_solution.pop_back();
						std::stringstream training_case_solution_stream;
						training_case_solution_stream.str(training_case_solution_str);

						while (training_case_solution_stream >> row)
						{
							for (int n = 0; n < row.size(); n++)
								training_case_solution.push_back(std::stod(row[n]));
						}

						training_cases_solution[training_case_index] = training_case_solution;

						training_case_index++;
						fitness_case_count--;
					}

					while ((test_case_index < argmap::number_of_test_cases) && (fitness_case_count > 0))
					{
						std::vector<double> test_case_problem;
						std::vector<double> test_case_solution;
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
								test_case_problem.push_back(std::stod(row[n]));
						}

						test_cases_problem[test_case_index] = test_case_problem;

						// Get solution
						test_case_solution_str = fitness_cases_solution.back();
						fitness_cases_solution.pop_back();
						std::stringstream test_case_solution_stream;
						test_case_solution_stream.str(test_case_solution_str);

						while (test_case_solution_stream >> row)
						{
							for (int n = 0; n < row.size(); n++)
								test_case_solution.push_back(std::stod(row[n]));
						}

						test_cases_solution[test_case_index] = test_case_solution;

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
					std::vector<double> training_case_input;
					std::vector<double> training_case_output;

					int training_case_length = pushGP::random_integer(argmap::example_case_max_length) + 1;

					for (int j = 0; j < training_case_length; j++)
					{
						int n = pushGP::random_integer(argmap::example_case_upper_range);
						training_case_input.push_back(n);
						training_case_output.push_back(n);
					}

					std::sort(training_case_output.begin(), training_case_output.end());

					training_cases_problem[i] = training_case_input;
					training_cases_solution[i] = training_case_output;

					training_cases_created++;
				}
			}

			if ((_example_cases_loaded + training_cases_created) < argmap::number_of_training_cases + argmap::number_of_test_cases)
			{
				for (int i = _example_cases_loaded + training_cases_created - argmap::number_of_training_cases; i < argmap::number_of_test_cases; i++)
				{
					std::vector<double> test_case_input;
					std::vector<double> test_case_output;

					int test_case_length = pushGP::random_integer(argmap::example_case_max_length) + 1;

					for (int j = 0; j < test_case_length; j++)
					{
						int n = pushGP::random_integer(argmap::example_case_upper_range);
						test_case_input.push_back(n);
						test_case_output.push_back(n);
					}

					std::sort(test_case_output.begin(), test_case_output.end());

					test_cases_problem[i] = test_case_input;
					test_cases_solution[i] = test_case_output;

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
			sqlcmd_insert_new_example_case->begin_transaction();  //transaction->begin();

			// Delete previously saved test cases
			sqlcmd_insert_new_example_case->execute(sqlstmt_delete_all_example_cases);

			// Save new 
			sqlcmd_insert_new_example_case->set_command(sqlstmt_insert_new_example_case);

			for (int i = 0; i < argmap::number_of_training_cases; i++)
			{
				std::vector<double> training_case_input;
				std::vector<double> training_case_output;

				training_case_input = training_cases_problem[i];
				training_case_output = training_cases_solution[i];

				std::string training_case_input_str = "";
				std::string training_case_element = "";

				for (int training_case_data : training_case_input)
				{
					if (training_case_input_str.length() > 0)
						training_case_input_str += ",";

					_itoa_s(training_case_data, buffer, 10);
					training_case_input_str += buffer;
				}

				std::string training_case_output_str = "";
				training_case_element = "";
				
				for (int training_case_data : training_case_output)
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
				std::vector<double> test_case_input;
				std::vector<double> test_case_output;

				test_case_input = test_cases_problem[i];
				test_case_output = test_cases_solution[i];

				std::string test_case_input_str = "";
				std::string test_case_element = "";

				for (int test_case_data : test_case_input)
				{
					if (test_case_input_str.length() > 0)
						test_case_input_str += ",";

					_itoa_s(test_case_data, buffer, 10);
					test_case_input_str += buffer;
				}

				std::string test_case_output_str = "";
				test_case_element = "";

				for (int test_case_data : test_case_output)
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
			sqlcmd_insert_new_example_case->commit_transaction();  //transaction->commit();

			delete sqlcmd_insert_new_example_case;
		}

		// Remarks:
		//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
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

						std::string genome = sqlcmd_get_individuals->get_field_as_string(2);
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

		// Remarks:
		//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
		unsigned int make_pop_agents(int _start)
		{
			unsigned int agents_created = 0;

			for (int n = _start; n < argmap::population_size; n++)
			{
				pushGP::globals::population_agents[n].set_genome(pushGP::random_plush_genome());

				agents_created++;
			}

			// Cleanup thread factories
			Push::env.clear_stacks();

			return agents_created;
		}

		void save_generation()
		{
			UUID NilUuid;

			// creates a nil-valued UUID
			UuidCreateNil(&NilUuid);

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

				sqlcmd_insert_new_individual->execute();
			}

			// Commit transaction
			sqlcmd_insert_new_individual->commit_transaction();  //transaction->commit();

			delete sqlcmd_delete_individuals;
			delete sqlcmd_insert_new_individual;
		}


		// Remarks:
		//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
		unsigned int verify_pop_agents()
		{
			unsigned int n = 0;

			// Setup
			database::SQLCommand* sqlcmd_get_individuals;

			sqlcmd_get_individuals = new database::SQLCommand(&con, sqlstmt_sqlcmd_get_individuals);

			try
			{
				std::cout << "verify_pop_agents()" << std::endl;

				sqlcmd_get_individuals->execute();			// Create thread factories

				if (sqlcmd_get_individuals->is_result_set())
				{
					while ((sqlcmd_get_individuals->fetch_next()) && (n < argmap::population_size))
					{
						// Check Plush Genome
						long individual_id = sqlcmd_get_individuals->get_field_as_long(1);
						std::string genome_string_from_db = trim_copy(sqlcmd_get_individuals->get_field_as_string(2));
						std::string genome_string_from_individual = trim_copy(pushGP::globals::population_agents[n].get_genome_as_string());

						if (genome_string_from_db != genome_string_from_individual)
						{
							std::cout << "Genome mismatch." << std::endl;
							std::cout << "  n = " << n << std::endl;
							std::cout << "  individual_id = " << n << std::endl;
							std::cout << "  Genome = " << genome_string_from_individual << std::endl;
							std::cout << "  Loaded = " << genome_string_from_db << std::endl;
							std::cout << std::endl;
						}

						// Check Push Program
						Genome::Genome genome(genome_string_from_db);
						std::string program_string_from_db = trim_copy(genome.get_program());
						std::string program_string_from_individual = trim_copy(pushGP::globals::population_agents[n].get_program());

						if (program_string_from_db != program_string_from_individual)
						{
							std::cout << "Program mismatch." << std::endl;
							std::cout << "  n = " << n << std::endl;
							std::cout << "  individual_id = " << n << std::endl;
							std::cout << "  Program = " << program_string_from_individual << std::endl;
							std::cout << "  Loaded = " << program_string_from_db << std::endl;
							std::cout << std::endl;
						}

						n++;
					}
				}

				else
					throw MyException("verify_pop_agents() - No result set");
			}
			catch (...)
			{
				delete sqlcmd_get_individuals;

				return n;
			}

			delete sqlcmd_get_individuals;

			return n;
		}

		int compute_errors(std::function<double(static unsigned int _individual_index, 
									static std::vector<double>& _input_list, 
									static std::vector<double>& _output_list)> _run_individual_program,
			int _number_of_example_cases) 
		{
			int individual_with_least_error = -1;
			int individual_with_best_score = -1;
			double min_error = (std::numeric_limits<double>::max)();
			double min_score = (std::numeric_limits<double>::max)();

			for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{
				int error_count= 0;

				if (individual_index == 48)
					error_count = 0;

				if ((individual_index % 100) == 0)
					std::cout << individual_index;

				pushGP::globals::minimum_error_array_by_individual[individual_index] = (std::numeric_limits<double>::max)();

				for (int example_case = 0; example_case < _number_of_example_cases; example_case++)
				{
					Push::intLiteralFactory->clean_up();
					Push::floatLiteralFactory->clean_up();
					Push::boolLiteralFactory->clean_up();
					Push::codeListFactory->clean_up();
					Push::doRangeClassFactory->clean_up();

					std::vector<double> example_problem = training_cases_problem[example_case];
					std::vector<double> example_solution = training_cases_solution[example_case];

					if ((individual_index == 48) && (example_case == 0))
						individual_index = 48;

					pushGP::globals::error_matrix[example_case][individual_index] = _run_individual_program(individual_index, example_problem, example_solution);

					if (pushGP::globals::error_matrix[example_case][individual_index] > 0.0)
						error_count++;

					// Set error array to the minimum error for all example cases
					if (pushGP::globals::minimum_error_array_by_individual[individual_index] > pushGP::globals::error_matrix[example_case][individual_index])
						pushGP::globals::minimum_error_array_by_individual[individual_index] = pushGP::globals::error_matrix[example_case][individual_index];
				}

				score_array[individual_index] = (double)error_count / (double)_number_of_example_cases;

				if ((score_array[individual_index] < 1.0) && (score_array[individual_index] < min_score))
				{
					min_score = score_array[individual_index];
					individual_with_best_score = individual_index;
				}

				if (pushGP::globals::minimum_error_array_by_individual[individual_index] < min_error)
				{
					min_error = pushGP::globals::minimum_error_array_by_individual[individual_index];
					individual_with_least_error = individual_index;
				}

				if ((individual_index % 100) == 0)
					std::cout << std::endl;
			}


			std::cout << std::endl;

			return (individual_with_best_score == -1) ? individual_with_least_error : individual_with_best_score;
		}

		double compute_errors(std::function<double(static unsigned int _individual_index,
								static std::vector<double>& _example_problem, 
			                    static std::vector<double>& _example_solution)> _run_individual_program,
			int _number_of_example_cases,
			static std::vector<double> _example_cases_problem[],
			static std::vector<double> _example_cases_solution[],
			int _individual_index)
		{
			double error = (std::numeric_limits<double>::max)();
			int error_count = 0;

			for (int example_case = 0; example_case < _number_of_example_cases; example_case++)
			{
				std::cout << ".";

				std::vector<double> example_problem = _example_cases_problem[example_case];
				std::vector<double> example_solution = _example_cases_solution[example_case];

				double example_case_error = _run_individual_program(_individual_index, example_problem, example_solution);

				if (example_case_error > 0.0)
					error_count++;
			}

			error = (double)error_count / (double)_number_of_example_cases;

			std::cout << std::endl;
			std::cout << std::endl;

			return error;
		}

		double compute_errors(std::function<double(std::string _genome, static std::vector<double>& _example_problem, static std::vector<double>& _example_solution)> _run_genome,
			int _number_of_example_cases,
			static std::vector<double> _example_cases_problem[],
			static std::vector<double> _example_cases_solution[],
			std::string _genome)
		{
			double error = (std::numeric_limits<double>::max)();
			int error_count = 0;

			for (int example_case = 0; example_case < _number_of_example_cases; example_case++)
			{
				std::cout << ".";

				std::vector<double> example_problem = _example_cases_problem[example_case];
				std::vector<double> example_solution = _example_cases_solution[example_case];

				double example_case_error = _run_genome(_genome, example_problem, example_solution);

				if (example_case_error > 0.0)
					error_count++;
			}

			error = (double)error_count / (double)_number_of_example_cases;

			std::cout << std::endl;
			std::cout << std::endl;

			return error;
		}

		pushGP::AsyncBreed async_breed;

		//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
		//
		void produce_new_offspring(int _number_of_example_cases, unsigned int _best_individual)
		{
			std::set<std::string> set_of_gnomes;
			std::pair<std::set<std::string>::iterator, bool> ret;

			// Reset children.
			std::cout << "  Reset children" << std::endl;
			for (unsigned int n = 0; n < argmap::population_size; n++)
				pushGP::globals::child_agents[n].clear_genome();

			// Breed new generation
			std::cout << "  Breed new generation" << std::endl;

			if (argmap::use_single_thread)
			{
				for (unsigned int individual_index = 0; individual_index < argmap::population_size; individual_index++)
				{
					pushGP::breed(individual_index, _number_of_example_cases);

					ret = set_of_gnomes.insert(pushGP::globals::child_agents[individual_index].get_genome_as_string());

					// If a child with the same genome already exists, create a new random child.
					if (ret.second == false)
						pushGP::globals::child_agents[individual_index].set_genome(pushGP::random_plush_genome());
				}
			}

			else
			{
				int num_threads = std::thread::hardware_concurrency() - argmap::number_of_cores_to_reserve;
				std::vector<std::thread> thread_pool;

				std::cout << "  Number of threads = " << num_threads << std::endl;

				// See https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-3.html
				for (int i = 0; i < num_threads - 1; i++)
					thread_pool.push_back(std::thread(&pushGP::AsyncBreed::breed_function_thread_pool, &async_breed));

				std::cout << "  Evaluate Individuals";

				for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
					async_breed.push(individual_index, _number_of_example_cases);

				async_breed.done();

				for (unsigned int i = 0; i < thread_pool.size(); i++)
				{
					thread_pool.at(i).join();
				}

				std::cout << std::endl;

				std::cout << "  Remove duplicates" << std::endl;
				for (unsigned int individual_index = 0; individual_index < argmap::population_size; individual_index++)
				{
					ret = set_of_gnomes.insert(pushGP::globals::child_agents[individual_index].get_genome_as_string());

					// If a child with the same genome already exists, create a new random child.
					if (ret.second == false)
						pushGP::globals::child_agents[individual_index].set_genome(pushGP::random_plush_genome());
				}
			}

			// Keep the best individuals
			pushGP::globals::child_agents[_best_individual].copy(pushGP::globals::population_agents[_best_individual]);

			for (unsigned int training_case = 0; training_case < domain::argmap::number_of_training_cases; training_case++)
			{
				unsigned int best_individual_for_training_case = pushGP::globals::individual_with_minimum_error_for_training_case[training_case];

				if (best_individual_for_training_case < (std::numeric_limits<unsigned int>::max)())
					pushGP::globals::child_agents[best_individual_for_training_case].copy(pushGP::globals::population_agents[best_individual_for_training_case]);
			}

			std::cout << std::endl;
		}

		void install_next_generation()
		{
			for (unsigned int n = 0; n < argmap::population_size; n++)
				pushGP::globals::population_agents[n].copy(pushGP::globals::child_agents[n]);
		}

		//void generate_status_report(unsigned int _generation_number,
		//	unsigned int _generations_completed_this_session, 
		//	unsigned int _best_individual_id, 
		//	double _best_individual_training_score, 
		//	double _best_individual_training_error,
		//	double _average_traiing_error,
		//	double _best_individual_test_score,
		//	std::string _best_gnome)
		//{
		//	database::SQLCommand* sqlcmd_save_status_report;

		//	sqlcmd_save_status_report = new database::SQLCommand(&con, sqlstmt_save_status_report);

		//	sqlcmd_save_status_report->set_as_integer(1, _generation_number);
		//	sqlcmd_save_status_report->set_as_integer(2, _generations_completed_this_session);
		//	sqlcmd_save_status_report->set_as_integer(3, _best_individual_id);
		//	sqlcmd_save_status_report->set_as_float(4, _best_individual_training_score);
		//	sqlcmd_save_status_report->set_as_float(5, _best_individual_training_error);
		//	sqlcmd_save_status_report->set_as_float(6, _average_traiing_error);
		//	sqlcmd_save_status_report->set_as_float(7, _best_individual_test_score);
		//	sqlcmd_save_status_report->set_as_integer(8, argmap::number_of_training_cases);
		//	sqlcmd_save_status_report->set_as_integer(9, argmap::number_of_test_cases);
		//	sqlcmd_save_status_report->set_as_string(10, _best_gnome);
		//	sqlcmd_save_status_report->set_as_integer(11, argmap::population_size);
		//	sqlcmd_save_status_report->set_as_float(12, argmap::alternation_rate);
		//	sqlcmd_save_status_report->set_as_float(13, argmap::uniform_mutation_rate);
		//	sqlcmd_save_status_report->set_as_integer(14, argmap::example_case_max_length);
		//	sqlcmd_save_status_report->set_as_integer(15, argmap::example_case_upper_range);

		//	sqlcmd_save_status_report->execute();

		//	delete sqlcmd_save_status_report;
		//}


		void generate_status_report(unsigned int _generation_number,
			unsigned int _generations_completed_this_session,
			unsigned int _best_individual_id,
			double _best_individual_training_score,
			double _best_individual_training_error,
			double _average_traiing_error,
			double _best_individual_test_score,
			std::string _best_gnome)
		{
			database::SQLCommand* sqlcmd_save_status_report;

			sqlcmd_save_status_report = new database::SQLCommand(&con, sqlstmt_save_status_report);

			sqlcmd_save_status_report->set_as_integer(1, _generation_number);
			sqlcmd_save_status_report->set_as_integer(2, _generations_completed_this_session);
			sqlcmd_save_status_report->set_as_integer(3, _best_individual_id);
			sqlcmd_save_status_report->set_as_float(4, _best_individual_training_score);
			sqlcmd_save_status_report->set_as_float(5, _best_individual_training_error);
			sqlcmd_save_status_report->set_as_float(6, _average_traiing_error);
			sqlcmd_save_status_report->set_as_float(7, _best_individual_test_score);
			sqlcmd_save_status_report->set_as_integer(8, argmap::number_of_training_cases);
			sqlcmd_save_status_report->set_as_integer(9, argmap::number_of_test_cases);
			sqlcmd_save_status_report->set_as_string(10, _best_gnome);
			sqlcmd_save_status_report->set_as_integer(11, argmap::population_size);
			sqlcmd_save_status_report->set_as_float(12, argmap::alternation_rate);
			sqlcmd_save_status_report->set_as_float(13, argmap::uniform_mutation_rate);
			sqlcmd_save_status_report->set_as_integer(14, argmap::example_case_max_length);
			sqlcmd_save_status_report->set_as_integer(15, argmap::example_case_upper_range);

			sqlcmd_save_status_report->execute();

			delete sqlcmd_save_status_report;
		}



//		int run(int argc, char** argv)
		int run()
		{
			// Create memory management factories
			Push::intLiteralFactory = new Push::LiteralFactory<int>();
			Push::floatLiteralFactory = new Push::LiteralFactory<double>();
			Push::boolLiteralFactory = new Push::LiteralFactory<bool>();
			Push::codeListFactory = new Push::CodeListFactory();
			Push::doRangeClassFactory = new Push::DoRangeClassFactory();

			// Setup
			Push::init_push();

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
				agents_created = make_pop_agents(load_pop_agents());

				if (agents_created > 0)
					generation_number = 0;

				while ((!done) && (generations_completed_this_session < argmap::max_generations_in_one_session))
				{
					// Reset variables which track the minimum error for this test case and the individual who achived the minimum error 
					std::cout << "Reset variables which track the minimum error for this test case and the individual who achived the minimum error " << std::endl;

					for (unsigned int example_case = 0; example_case < argmap::number_of_training_cases; example_case++)
					{
						pushGP::globals::minimum_error_array_by_example_case[example_case] = (std::numeric_limits<double>::max)();
						pushGP::globals::individual_with_minimum_error_for_training_case[example_case] = (std::numeric_limits<unsigned int>::max)();
					}

					for (int ind = 0; ind < argmap::population_size; ind++)
					{
						for (int training_case_index = 0; training_case_index < argmap::number_of_training_cases; training_case_index++)
							pushGP::globals::error_matrix[training_case_index][ind] = 0.0;
					}

					std::cout << "Clean up memory" << std::endl;

					Push::intLiteralFactory->clean_up();
					Push::floatLiteralFactory->clean_up();
					Push::boolLiteralFactory->clean_up();
					Push::codeListFactory->clean_up();
					Push::doRangeClassFactory->clean_up();

					std::cout << "Generation " << generation_number << std::endl;
					std::cout << "Session " << generations_completed_this_session << std::endl;
					save_generation();
//					verify_pop_agents();

					std::cout << "Run Programs with Training Cases" << std::endl;
					int best_individual = compute_errors(run_individual, argmap::number_of_training_cases);

					std::cout << "Produce New Offspring" << std::endl;
					produce_new_offspring(argmap::number_of_training_cases,	best_individual);

					std::cout << "Run Best Individual's Program with Test Cases" << std::endl;
					
					std::string program = pushGP::globals::population_agents[best_individual].get_program();
					std::string genome = pushGP::globals::population_agents[best_individual].get_genome_as_string();

					std::cout << "best_individual = " << best_individual << std::endl;
					std::cout << "program = " << program << std::endl;
					std::cout << "genome = " << genome << std::endl;

					double test_case_score = compute_errors(run_individual,
						argmap::number_of_test_cases,
						test_cases_problem,
						test_cases_solution,
						best_individual);

					std::cout << "test_case_error = " << test_case_score << std::endl;
					std::cout << std::endl;

					std::cout << "Verify Best Individual's Program with Test Cases" << std::endl;

					//std::string genome = "{:instruction EXEC.DO*RANGE :close  0}{:instruction FLOAT.SWAP :close  0}{:instruction FLOAT.- :close  0}{:instruction FLOAT.FROMINTEGER :close  0}{:instruction INTEGER.FLUSH :close  0}{:instruction INTEGER.> :close  0}{:instruction CODE.DUP :close  0}{:instruction BOOLEAN.NOR :close  0}{:instruction FLOAT.YANK :close  0}{:instruction INTEGER.FROMFLOAT :close  0}{:instruction FLOAT./ :close  4}{:instruction FLOAT.FLUSH :close  0}{:instruction EXEC.IF :close  0}";

					double verify_test_case_error = compute_errors(run_genome,
						argmap::number_of_test_cases,
						test_cases_problem,
						test_cases_solution,
						genome);
									   					 				  
					std::cout << "verify_test_case_error = " << verify_test_case_error << std::endl;
					std::cout << std::endl;

					std::cout << "Generate Status Report" << std::endl;

					double average_traiing_error = 0.0;
					for (int ind = 0; ind < argmap::population_size; ind++)
					{
						for (int training_case_index = 0; training_case_index < argmap::number_of_training_cases; training_case_index++)
							average_traiing_error += pushGP::globals::error_matrix[training_case_index][ind];
					}
					average_traiing_error /= (double)(domain::argmap::population_size * argmap::number_of_training_cases);

					generate_status_report(generation_number, 
						generations_completed_this_session, 
						best_individual, 
						score_array[best_individual],
						pushGP::globals::minimum_error_array_by_individual[best_individual],
						average_traiing_error,
						test_case_score, 
						pushGP::globals::population_agents[best_individual]);

					std::cout << "Install New Generation" << std::endl;
					install_next_generation();
					generation_number++;
					generations_completed_this_session++;
				}
			}
			catch (const std::exception& e)
			{
				// Cleanup thread factories
				Push::env.clear_stacks();

				delete Push::intLiteralFactory;
				delete Push::floatLiteralFactory;
				delete Push::boolLiteralFactory;
				delete Push::codeListFactory;
				delete Push::doRangeClassFactory;

				std::cout << "Standard exception: " << e.what() << std::endl;
				throw;
			}
			catch (...)
			{
				// Cleanup thread factories
				Push::env.clear_stacks();

				delete Push::intLiteralFactory;
				delete Push::floatLiteralFactory;
				delete Push::boolLiteralFactory;
				delete Push::codeListFactory;
				delete Push::doRangeClassFactory;

				std::cout << "Exception occurred" << std::endl;
				throw;
			}

			delete Push::intLiteralFactory;
			delete Push::floatLiteralFactory;
			delete Push::boolLiteralFactory;
			delete Push::codeListFactory;
			delete Push::doRangeClassFactory;

			return 0;
		}
	}
}
