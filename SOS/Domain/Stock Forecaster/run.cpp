#include <iostream>
#include <numeric>

#include "run.h"
//#include "Globals.h"
#include "ErrorFunction.h"
#include "Broker.h"
#include "PushFunctions.h"
#include "AsyncErrorFunction.h"
#include "../../PushP/Literal.h"
#include "../../PushP/ExecInstruction.h"
#include "../../Database/SQLCommand.h"
#include "../../Database/SQLField.h"
#include "../../PushGP/Globals.h"
#include "../../PushGP/Random.h"
#include "../../PushGP/Breed.h"
#include "../../PushGP/Selection.h"
#include "../../PushP/Literal.h"

namespace domain
{
	namespace stock_forecaster
	{
		database::SQLConnection con;

		const std::string sqlstmt_get_last_saved_generation_number = "SELECT TOP 1 [Generation] FROM [SOS].[dbo].[ProgressLog] ORDER BY[Update_DTS] DESC;";
		const std::string sqlstmt_sqlcmd_get_individuals = "SELECT [Genome] FROM [dbo].[Individuals];";
		const std::string sqlstmt_delete_individuals("DELETE FROM [SOS].[dbo].[Individuals];");
		const std::string sqlstmt_insert_new_individual("INSERT INTO [dbo].[Individuals] ([Genome]) VALUES (?);");
		const std::string sqlstmt_save_status_report("INSERT INTO [dbo].[ProgressLog]"
			"           ("
			"            [Generation]"								// 1
			"           ,[Group_TrainingScore]"						// 2
			"           ,[Group_TestScore]"							// 3			
			"           ,[EligibleParents_TrainingScore]"			// 4
			"           ,[EligibleParents_TestScore]"				// 5
			"           ,[TestCase_Best_Individuals_TrainingScore]"	// 6
			"           ,[TestCase_Best_leIndividuals_TestScore]"	// 7
			"           ,[BestIndividual_TrainingScore]"			// 8
			"           ,[BestIndividual_TestScore]"				// 9
			"           ,[Training_Sscore_of_Eelite_Individual_with_Maximum_Number_Test_Cases]"	// 10
			"           ,[Test_Sscore_of_Eelite_Individual_with_Maximum_Number_Test_Cases]"		// 11
			"           ,[Elite_Size]"								// 12
			"           ,[Elite_TestCases]"							// 13
			"           ,[Total_TestCases]"							// 14
			"           ,[Opening_Balance]"							// 15
			"           ,[Population_Size]"							// 16
			"           ,[Alternation_Rate]"						// 17
			"           ,[Uniform_Mutation_Rate]"					// 18
			"           ,[Genome_of_individual_with_best_training_score_for_all_data]"									// 19
			"           ,[Genome_of_elite_individual_with_maximum_number_test_cases]"									// 20
			"           ,[Minimum_Epsilon]"									// 21
			"           ,[Avgerage_Epsilon]"									// 22
			"           ,[Maximum_Epsilon]"									// 23
			"           ,[Minimum_Non_Zero_Epsilons]"									// 24
			"           ,[Average_Non_Zero_Epsilons]"									// 25
			"           ,[Maximum_Non_Zero_Epsilons]"									// 26
			"           )"
			"     VALUES"
			"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		//       1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6

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

						for (int i = 0; i < argmap::last_data_index; i++)
							globals::order_bank[n][i] = globals::order_types::not_available;

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

				for (int i = 0; i < argmap::last_data_index; i++)
					globals::order_bank[n][i] = globals::order_types::not_available;

				agents_created++;
			}

			return agents_created;
		}

		void compute_errors(std::function<double(unsigned int, unsigned long, unsigned long)> _reproduction_selection_error_function, unsigned long _input_start, unsigned long _input_end)
		{
			AsyncErrorFunction async_error_function;

			if (argmap::use_single_thread)
			{
				double min_error = std::numeric_limits<double>::max();

				for (int n = 0; n < argmap::population_size; n++)
				{
					std::cout << "  Evaluate Individual " << n;
					double error = _reproduction_selection_error_function(n, _input_start, _input_end);
					std::cout << " Min error = " << error << std::endl;
					min_error = min_error < error ? min_error : error;
				}

				std::cout << "   Min error = " << min_error << std::endl;
			}

			else
			{
				int num_threads = std::thread::hardware_concurrency() - argmap::number_of_cores_to_reserve;
				std::vector<std::thread> thread_pool;

				std::cout << "  Number of threads = " << num_threads << std::endl;

				for (int i = 0; i < num_threads - 1; i++)
					thread_pool.push_back(std::thread(&AsyncErrorFunction::reproduction_selection_error_function_thread_pool, &async_error_function, _reproduction_selection_error_function));


				std::cout << "  Evaluate Individuals";

				for (int n = 0; n < argmap::population_size; n++)
				{
					async_error_function.push(n, _input_start, _input_end);
				}

				async_error_function.done();

				for (unsigned int i = 0; i < thread_pool.size(); i++)
				{
					thread_pool.at(i).join();
				}

				std::cout << std::endl;
			}
		}

		void produce_new_offspring()
		{
			//std::unordered_set<std::string> set_of_gnomes;
			//std::pair<std::unordered_set<std::string>::iterator, bool> ret;
			std::set<std::string> set_of_gnomes;
			std::pair<std::set<std::string>::iterator, bool> ret;

			// Reset children.
			for (unsigned int n = 0; n < argmap::population_size; n++)
				pushGP::globals::child_agents[n].clear_genome();

			for (unsigned int n = 0; n < argmap::population_size; n++)
			{
				if (pushGP::globals::child_agents[n].get_genome().empty())
				{
					ret = set_of_gnomes.insert(pushGP::breed(pushGP::globals::child_agents[n]).get_genome_as_string());

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
			{
				pushGP::globals::population_agents[n].set(pushGP::globals::child_agents[n]);

				for (int i = 0; i < argmap::last_data_index; i++)
					globals::order_bank[n][i] = globals::order_types::not_available;
			}
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

		void generate_status_report(int generation_,
			std::function<double(static std::vector<int> & individual_indexes, 
				static unsigned long input_start, 
				static unsigned long input_end, 
				unsigned int _test_case, bool _record_transactions)> individual_selection_error_function,
			unsigned int training_input_start,
			unsigned int training_input_end,
			unsigned int test_input_start,
			unsigned int test_input_end)
		{
			unsigned int n = 0;
			double min_error = std::numeric_limits<double>::max();
			double max_error_for_all_individuals_for_all_data = std::numeric_limits<double>::min();
			int index_of_individual_with_best_training_score_for_all_data = 0;
			database::SQLCommand* sqlcmd_save_status_report;

			double training_score_of_individual_with_best_training_score_for_all_data = 0;
			double validation_score_of_individual_with_best_training_score_for_all_data = 0;

			sqlcmd_save_status_report = new database::SQLCommand(&con, sqlstmt_save_status_report);

			// Calculate the best individual's training score
			// Clear test case counts

			pushGP::AsyncErrorFunction async_error_function;

			if (argmap::use_single_thread)
			{
				for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
				{
					pushGP::globals::population_agents[individual_index].clear_elite_test_cases();

					std::cout << "Calculate the best individual's training score for individual #" << individual_index + 1 << std::endl;

					std::vector<int> individual_indexes = { individual_index };

					double error = individual_selection_error_function(individual_indexes, training_input_start, training_input_end, 0, false);

					pushGP::globals::population_agents[individual_index].set_error_for_all_training_data(error);

					if (error < min_error)
					{
						min_error = error;
						index_of_individual_with_best_training_score_for_all_data = individual_index;
					}

					if (error > max_error_for_all_individuals_for_all_data)
						max_error_for_all_individuals_for_all_data = error;
				}
			}
			else
			{
				int num_threads = std::thread::hardware_concurrency() - argmap::number_of_cores_to_reserve;
				std::vector<std::thread> thread_pool;

				for (int i = 0; i < num_threads - 1; i++)
					thread_pool.push_back(std::thread(&pushGP::AsyncErrorFunction::individual_selection_error_function_thread_pool, &async_error_function, individual_selection_error_function));

				std::cout << "Calculate the best individual's training score";

				for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
				{
					pushGP::globals::population_agents[individual_index].clear_elite_test_cases();

					std::vector<int> individual_indexes = { individual_index };

					async_error_function.push(individual_indexes, training_input_start, training_input_end, 0, false);
				}

				async_error_function.done();

				for (unsigned int i = 0; i < thread_pool.size(); i++)
					thread_pool.at(i).join();

				std::cout << std::endl;

				min_error = async_error_function.min_error();
				max_error_for_all_individuals_for_all_data = async_error_function.max_error_for_all_individuals_for_all_data();

				index_of_individual_with_best_training_score_for_all_data = async_error_function.min_error_individual_index();
			}

			training_score_of_individual_with_best_training_score_for_all_data = 0.0 - min_error;

			std::cout << "Best individual's Training Score = " << training_score_of_individual_with_best_training_score_for_all_data << std::endl;

			// Calculate the best individual's test score
			std::vector<int> best_individual_indexes = { index_of_individual_with_best_training_score_for_all_data };
			double error = individual_selection_error_function(best_individual_indexes, test_input_start, test_input_end, 0, false);
			validation_score_of_individual_with_best_training_score_for_all_data = 0.0 - error;

			std::cout << "Best individual's Test Score = " << validation_score_of_individual_with_best_training_score_for_all_data << std::endl;

			// Find the individual with the minimum error for each test case
			std::vector<double> test_case_minimum_error(Number_Of_Test_Cases);
			std::vector<int> index_of_best_individual_for_each_test_case(Number_Of_Test_Cases);
			std::set<unsigned int> set_of_eligible_parents;
			std::vector<int> index_of_eligible_parents;

			for (int test_case_index = 0; test_case_index < Number_Of_Test_Cases; test_case_index++)
			{
				// Set elite to the minimum error
				test_case_minimum_error[test_case_index] = std::numeric_limits<double>::max();
				index_of_best_individual_for_each_test_case[test_case_index] = -1;	// Initialize to refer to a non-existing individual

				for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
				{
					if (pushGP::globals::population_agents[individual_index].get_errors()[test_case_index] < test_case_minimum_error[test_case_index])
					{
						test_case_minimum_error[test_case_index] = pushGP::globals::population_agents[individual_index].get_errors()[test_case_index];
						index_of_best_individual_for_each_test_case[test_case_index] = individual_index;
					}

					if ((test_case_minimum_error[test_case_index] < 0.0) //std::numeric_limits<double>::max())
						&& (pushGP::globals::population_agents[individual_index].get_errors()[test_case_index] <= (test_case_minimum_error[test_case_index] + pushGP::globals::epsilons[test_case_index]))
						)
					{
						set_of_eligible_parents.insert(individual_index);
						pushGP::globals::population_agents[individual_index].log_elite_test_case(test_case_index);
					}
				}
			}

			for (int individual_index : set_of_eligible_parents)
				index_of_eligible_parents.push_back(individual_index);

			// Calculate the training error from the best individuals from each test case
			std::cout << "Training error from the best individuals from each test case = ";
			error = individual_selection_error_function(index_of_best_individual_for_each_test_case, training_input_start, training_input_end, 0, false);
			double best_individual_for_each_test_case_group_training_score = 0.0 - error;
			std::cout << best_individual_for_each_test_case_group_training_score << std::endl;
			std::cout << std::endl;

			// Calculate the test error from the best individuals from each test case
			std::cout << "Test error from the best individuals from each test case = ";
			error = individual_selection_error_function(index_of_best_individual_for_each_test_case, test_input_start, test_input_end, 0, false);
			double best_individual_for_each_test_case_group_validation_score = 0.0 - error;
			std::cout << best_individual_for_each_test_case_group_validation_score << std::endl;
			std::cout << std::endl;

			// Calculate the training error for the eligible parents
			std::cout << "Eligible parents training score = ";
			error = individual_selection_error_function(index_of_eligible_parents, training_input_start, training_input_end, 0, false);
			double eligible_parents_training_score = 0.0 - error;
			std::cout << eligible_parents_training_score << std::endl;
			std::cout << std::endl;

			// Calculate the test error for the eligible parents
			std::cout << "Eligible parents test score = ";
			error = individual_selection_error_function(index_of_eligible_parents, test_input_start, test_input_end, 0, false);
			double eligible_parents_validation_score = 0.0 - error;
			std::cout << eligible_parents_validation_score << std::endl;
			std::cout << std::endl;

			// Calculate group training score
			std::vector<int> index_of_individuals;

			for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
				index_of_individuals.push_back(individual_index);

			std::cout << "Group training score = ";
			error = individual_selection_error_function(index_of_individuals, training_input_start, training_input_end, 0, false);
			double group_training_score = 0.0 - error;
			std::cout << group_training_score << std::endl;

			// Calculte group test score
			std::cout << "Group test score = ";
			error = individual_selection_error_function(index_of_individuals, test_input_start, test_input_end, 0, false);
			double group_validation_score = 0.0 - error;
			std::cout << group_validation_score << std::endl;

			// Calculate number of individuals whom qualify as an elite individual			
			// Calculate maximum number of test cases for any elite individual
			int number_of_elite_individuals = 0;
			int maximum_number_of_test_cases_for_any_elite_individual = 0;
			int index_of_elite_individual_with_maximum_number_test_cases = 0;

			std::cout << "test_case_index, test_case_minimum_error[test_case_index], globals::epsilons[test_case_index], globals::non_zero_epsilons[test_case_index]" << std::endl;
			for (int test_case_index = 0; test_case_index < Number_Of_Test_Cases; test_case_index++)
				std::cout << test_case_index << ", "
				<< test_case_minimum_error[test_case_index] << ", "
				<< pushGP::globals::epsilons[test_case_index] << ","
				<< pushGP::globals::non_zero_epsilons[test_case_index] << std::endl;
			std::cout << std::endl;

			for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
			{
				for (int test_case_index = 0; test_case_index < Number_Of_Test_Cases; test_case_index++)
				{
					if (pushGP::globals::population_agents[individual_index].get_errors()[test_case_index] <= (test_case_minimum_error[test_case_index] + pushGP::globals::epsilons[test_case_index]))
					{
						pushGP::globals::population_agents[individual_index].make_elite();
						number_of_elite_individuals++;
						break;
					}
				}

				if (maximum_number_of_test_cases_for_any_elite_individual < pushGP::globals::population_agents[individual_index].count_elite_test_cases())
				{
					maximum_number_of_test_cases_for_any_elite_individual = pushGP::globals::population_agents[individual_index].count_elite_test_cases();
					index_of_elite_individual_with_maximum_number_test_cases = individual_index;
				}
			}

			// Calculate training score for elite individual with the maximum number of test cases
			std::cout << "training_score_of_elite_individual_with_maximum_number_test_cases = ";
			double training_score_of_elite_individual_with_maximum_number_test_cases = 0.0;
			index_of_individuals.clear();
			index_of_individuals.push_back(index_of_elite_individual_with_maximum_number_test_cases);

			error = individual_selection_error_function(index_of_individuals, training_input_start, training_input_end, 0, true);

			training_score_of_elite_individual_with_maximum_number_test_cases = 0.0 - error;
			std::cout << training_score_of_elite_individual_with_maximum_number_test_cases << std::endl;

			// Calculate test score for elite individual with the maximum number of test cases
			std::cout << "test_score_of_elite_individual_with_maximum_number_test_cases = ";
			double validation_score_of_elite_individual_with_maximum_number_test_cases = 0.0;

			error = individual_selection_error_function(index_of_individuals, test_input_start, test_input_end, -1, true);

			validation_score_of_elite_individual_with_maximum_number_test_cases = 0.0 - error;
			std::cout << validation_score_of_elite_individual_with_maximum_number_test_cases << std::endl;

			// Set parameters to save
			sqlcmd_save_status_report->set_as_integer(1, generation_);
			sqlcmd_save_status_report->set_as_float(2, group_training_score);
			sqlcmd_save_status_report->set_as_float(3, group_validation_score);
			sqlcmd_save_status_report->set_as_float(4, eligible_parents_training_score);
			sqlcmd_save_status_report->set_as_float(5, eligible_parents_validation_score);
			sqlcmd_save_status_report->set_as_float(6, best_individual_for_each_test_case_group_training_score);
			sqlcmd_save_status_report->set_as_float(7, best_individual_for_each_test_case_group_validation_score);
			sqlcmd_save_status_report->set_as_float(8, training_score_of_individual_with_best_training_score_for_all_data);
			sqlcmd_save_status_report->set_as_float(9, validation_score_of_individual_with_best_training_score_for_all_data);

			sqlcmd_save_status_report->set_as_float(10, training_score_of_elite_individual_with_maximum_number_test_cases);
			sqlcmd_save_status_report->set_as_float(11, validation_score_of_elite_individual_with_maximum_number_test_cases);

			sqlcmd_save_status_report->set_as_integer(12, number_of_elite_individuals);
			sqlcmd_save_status_report->set_as_integer(13, maximum_number_of_test_cases_for_any_elite_individual);
			sqlcmd_save_status_report->set_as_integer(14, Number_Of_Test_Cases);

			sqlcmd_save_status_report->set_as_float(15, argmap::opening_balance);
			sqlcmd_save_status_report->set_as_integer(16, argmap::population_size);
			sqlcmd_save_status_report->set_as_float(17, argmap::alternation_rate);
			sqlcmd_save_status_report->set_as_float(18, argmap::uniform_mutation_rate);
			sqlcmd_save_status_report->set_as_string(19, pushGP::globals::population_agents[index_of_individual_with_best_training_score_for_all_data].get_genome_as_string());
			sqlcmd_save_status_report->set_as_string(20, pushGP::globals::population_agents[index_of_elite_individual_with_maximum_number_test_cases].get_genome_as_string());

			auto minmax_epsilon = std::minmax_element(pushGP::globals::epsilons.begin(), pushGP::globals::epsilons.end());
			float average_epsilon = std::accumulate(pushGP::globals::epsilons.begin(), pushGP::globals::epsilons.end(), 0.0) / pushGP::globals::epsilons.size();

			sqlcmd_save_status_report->set_as_float(21, *minmax_epsilon.first);
			sqlcmd_save_status_report->set_as_float(22, average_epsilon);
			sqlcmd_save_status_report->set_as_float(23, *minmax_epsilon.second);

			auto minmax_non_zero_epsilons = std::minmax_element(pushGP::globals::non_zero_epsilons.begin(), pushGP::globals::non_zero_epsilons.end());
			float average_non_zero_epsilons = std::accumulate(pushGP::globals::non_zero_epsilons.begin(), pushGP::globals::non_zero_epsilons.end(), 0.0) / pushGP::globals::non_zero_epsilons.size();

			sqlcmd_save_status_report->set_as_integer(24, *minmax_non_zero_epsilons.first);
			sqlcmd_save_status_report->set_as_float(25, average_non_zero_epsilons);
			sqlcmd_save_status_report->set_as_integer(26, *minmax_non_zero_epsilons.second);

			sqlcmd_save_status_report->execute();

			delete sqlcmd_save_status_report;

			// Allow best individuals to survive
			if (argmap::error_ratio_cap_for_retaining_parents > 0.0)
			{
				pushGP::globals::child_agents[index_of_elite_individual_with_maximum_number_test_cases].set(pushGP::globals::population_agents[index_of_elite_individual_with_maximum_number_test_cases]);

				if (max_error_for_all_individuals_for_all_data > 0.0)
				{
					double cap = pushGP::random_double(argmap::error_ratio_cap_for_retaining_parents);

					for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
					{
						double ratio = pushGP::globals::population_agents[individual_index].get_error_for_all_training_data() / max_error_for_all_individuals_for_all_data;

						if (ratio < cap)
							pushGP::globals::child_agents[individual_index].set(pushGP::globals::population_agents[individual_index]);
					}
				}
			}

			// Save transaction log to file.
// EG
//			globals::population_agents[index_of_elite_individual_with_maximum_number_test_cases].dump_transactions();
		}

		int domain::stock_forecaster::run(int argc, char ** argv)
		{
			try
			{
				unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				unsigned int agents_created = 0;
				bool done = false;

				// Initialize database connection
				con.connect(argmap::db_init_datasource, argmap::db_init_catalog, argmap::db_user_id, argmap::db_user_password);

				// Create main factories
				Push::intLiteralFactory = new Push::LiteralFactory<int>();
				Push::floatLiteralFactory = new Push::LiteralFactory<double>();
				Push::boolLiteralFactory = new Push::LiteralFactory<bool>();
				Push::codeListFactory = new Push::CodeListFactory();
				Push::doRangeClassFactory = new Push::DoRangeClassFactory();

				// Create references to the main factories
				Push::LiteralFactory<int> *intLiteralFactory_old = Push::intLiteralFactory;
				Push::LiteralFactory<double> *floatLiteralFactory_old = Push::floatLiteralFactory;
				Push::LiteralFactory<bool> *boolLiteralFactory_old = Push::boolLiteralFactory;
				Push::CodeListFactory* codeListFactory_old = Push::codeListFactory;
				Push::DoRangeClassFactory* doRangeClassFactory_old = Push::doRangeClassFactory;

				// Initialze
				Broker::load_datatable();
				Push::init_push(init_push_application_specific_functions);
				Push::init_static_PushP_instructions();

				// Load population.  Create more if not enough loaded.
				std::cout << "Create Population Agents" << std::endl;
				generation_number = get_last_saved_generation_number() + 1;
				agents_created = make_pop_agents(load_pop_agents());

				if (agents_created > 0)
					generation_number = 0;

				//cout << "Create Child Agents" << endl;
				//make_child_agents();

				// Save references to the main factories
				intLiteralFactory_old = Push::intLiteralFactory;
				floatLiteralFactory_old = Push::floatLiteralFactory;
				boolLiteralFactory_old = Push::boolLiteralFactory;
				codeListFactory_old = Push::codeListFactory;
				doRangeClassFactory_old = Push::doRangeClassFactory;

				while ((!done) && (generations_completed_this_session < argmap::max_generations_in_one_session))
				{
					std::cout << "Generation " << generation_number << std::endl;
					std::cout << "Session " << generations_completed_this_session << std::endl;
					save_generation();

					std::cout << "Run Programs" << std::endl;
					compute_errors(run_individuals_program, argmap::training_start_index, argmap::last_data_index);

					std::cout << "Compte Errors" << std::endl;
					compute_errors(lexicase_reproduction_selection_error_function, argmap::training_start_index, argmap::training_end_index);

					std::cout << "Number_Of_Test_Cases = " << Number_Of_Test_Cases << std::endl;

					std::cout << "Calculate Epsilons" << std::endl;
					pushGP::calculate_epsilons_for_epsilon_lexicase();

					std::cout << "Produce New Offspring" << std::endl;
					produce_new_offspring();

					std::cout << "Generate status report" << std::endl;
					generate_status_report(generation_number,
						evaluate_individuals,
						argmap::training_start_index,
						argmap::training_end_index,
						argmap::test_start_index,
						argmap::test_end_index);

					std::cout << "Install New Generation" << std::endl;
					install_next_generation();
					generation_number++;
					generations_completed_this_session++;
				}

				// Restore old heap manager
				Push::intLiteralFactory = intLiteralFactory_old;
				Push::floatLiteralFactory = floatLiteralFactory_old;
				Push::boolLiteralFactory = boolLiteralFactory_old;
				Push::codeListFactory = codeListFactory_old;
				Push::doRangeClassFactory = doRangeClassFactory_old;

				// Cleanup main factories
				delete Push::intLiteralFactory;
				delete Push::floatLiteralFactory;
				delete Push::boolLiteralFactory;
				delete Push::codeListFactory;
				delete Push::doRangeClassFactory;
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
