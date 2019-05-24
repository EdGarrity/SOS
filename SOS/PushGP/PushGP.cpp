#include <mutex>
#include <functional>
#include <thread>

#include "PushGP.h"
#include "Globals.h"
#include "Individual.h"
#include "Random.h"
#include "Selection.h"
#include "Breed.h"
#include "..\PushP\StaticInit.h"
#include "..\PushP\Literal.h"
#include "..\PushP\ExecInstruction.h"
#include "..\Finance\Broker.h"
#include "..\Database/SQLCommand.h"
#include "..\Database/SQLField.h"
#include "AsyncErrorFunction.h"

using namespace std;
using namespace Push;
using namespace pushGP;

namespace pushGP
{
	database::SQLConnection con;


	const std::string sqlstmt_get_last_saved_generation_number = "SELECT TOP 1 [Generation] FROM[SOS].[dbo].[ProgressLog] ORDER BY[Update_DTS] DESC;";
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
		"           ,[Genome]"									// 19
		"           )"
		"     VALUES"
		"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
			//       1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9

	unsigned long get_last_saved_generation_number()
	{
		unsigned long n = 0;

//		database::SQLConnection con(argmap::db_init_datasource, argmap::db_init_catalog, argmap::db_user_id, argmap::db_user_password);

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
				while (sqlcmd_get_individuals->fetch_next())
				{
					std::cout << "n = " << n << std::endl;

					std::string ind = sqlcmd_get_individuals->get_field_as_string(1);
					globals::population_agents[n++].set_genome(ind);
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
			globals::population_agents[n].set_genome(random_plush_genome());
			agents_created++;
		}

		return agents_created;
	}

	//void make_child_agents()
	//{
	//	for (int n = 0; n < argmap::population_size; n++)
	//	{
	//		Individual individual = Individual();
	//		globals::child_agents[n] = individual;
	//	}
	//}

	void compute_errors(std::function<double(unsigned int, unsigned long, unsigned long)> reproduction_selection_error_function, unsigned long input_start, unsigned long input_end)
	{
		AsyncErrorFunction async_error_function;

		if (argmap::use_single_thread)
		{
			double min_error = std::numeric_limits<double>::max();

			for (int n = 0; n < argmap::population_size; n++)
			{
				cout << "  Evaluate Individual " << n;
				double error = reproduction_selection_error_function(n, input_start, input_end);
				cout << " Min error = " << error << std::endl;
				min_error = min_error < error ? min_error : error;
			}

			cout << "   Min error = " << min_error << std::endl;
		}

		else
		{
			int num_threads = std::thread::hardware_concurrency() - argmap::number_of_cores_to_reserve;
			std::vector<std::thread> thread_pool;

			cout << "  Number of threads = " << num_threads << std::endl;

			for (int i = 0; i < num_threads - 1; i++)
				thread_pool.push_back(std::thread(&AsyncErrorFunction::reproduction_selection_error_function_thread_pool, &async_error_function, reproduction_selection_error_function));


			cout << "  Evaluate Individuals";

			for (int n = 0; n < argmap::population_size; n++)
			{
				async_error_function.push(n, input_start, input_end);
			}

			async_error_function.done();

			for (unsigned int i = 0; i < thread_pool.size(); i++)
			{
				thread_pool.at(i).join();
			}

			cout << std::endl;
		}
	}

	void produce_new_offspring()
	{
		std::set<std::string> set_of_gnomes;
		std::pair<std::set<std::string>::iterator, bool> ret;

		for (unsigned int n = 0; n < argmap::population_size; n++)
		{
			ret = set_of_gnomes.insert(breed(globals::child_agents[n]).get_genome_as_string());

			// If a child with the same genome aalready exists, create a new random child.
			if (ret.second == false)
				globals::child_agents[n].set_genome(random_plush_genome());
		}
	}

	void install_next_generation()
	{
		//for (unsigned int n = 0; n < argmap::population_size; n++)
		//{
		//	globals::population_agents[n] = globals::child_agents[n];
		//}

		for (unsigned int n = 0; n < argmap::population_size; n++)
			globals::population_agents[n].set(globals::child_agents[n]);
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
			sqlcmd_insert_new_individual->set_as_string(1, globals::population_agents[n]);
			sqlcmd_insert_new_individual->execute();
		}

		// Commit transaction
		sqlcmd_insert_new_individual->commit_transaction();  //transaction->commit();

		delete sqlcmd_delete_indiciduals;
		delete sqlcmd_insert_new_individual;
	}

	void generate_status_report(int generation_, 
		std::function<double(static std::vector<int> & individual_indexes, static unsigned long input_start, static unsigned long input_end, unsigned int _test_case, bool _record_transactions)> individual_selection_error_function,
		unsigned int training_input_start, 
		unsigned int training_input_end,
		unsigned int test_input_start,
		unsigned int test_input_end)
	{
		unsigned int n = 0;
		double min_error = std::numeric_limits<double>::max();
		int index_of_individual_with_best_training_score_for_all_data = 0;
		database::SQLCommand* sqlcmd_save_status_report;

		double training_score_of_individual_with_best_training_score_for_all_data = 0;
		double validation_score_of_individual_with_best_training_score_for_all_data = 0;

//		database::SQLConnection con(argmap::db_init_datasource, argmap::db_init_catalog, argmap::db_user_id, argmap::db_user_password);

		sqlcmd_save_status_report = new database::SQLCommand(&con, sqlstmt_save_status_report);

		// Calculate the best individual's training score
		// Clear test case counts
		min_error = std::numeric_limits<double>::max();

		AsyncErrorFunction async_error_function;

		if (argmap::use_single_thread)
		{
			for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
			{
				globals::population_agents[individual_index].clear_elite_test_cases();

				std::cout << "Calculate the group training score for individual #" << individual_index + 1 << std::endl;

				std::vector<int> individual_indexes = { individual_index };

				double error = individual_selection_error_function(individual_indexes, training_input_start, training_input_end, 0, false);

				if (error < min_error)
				{
					min_error = error;
					index_of_individual_with_best_training_score_for_all_data = individual_index;
				}
			}
		}
		else
		{
			int num_threads = std::thread::hardware_concurrency() - argmap::number_of_cores_to_reserve;
			std::vector<std::thread> thread_pool;

			for (int i = 0; i < num_threads - 1; i++)
				thread_pool.push_back(std::thread(&AsyncErrorFunction::individual_selection_error_function_thread_pool, &async_error_function, individual_selection_error_function));

			std::cout << "Calculate the group training score";

			for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
			{
				globals::population_agents[individual_index].clear_elite_test_cases();

				std::vector<int> individual_indexes = { individual_index };

				async_error_function.push(individual_indexes, training_input_start, training_input_end, 0, false);
			}

			async_error_function.done();

			for (unsigned int i = 0; i < thread_pool.size(); i++)
				thread_pool.at(i).join();

			cout << std::endl;

			min_error = async_error_function.min_error();
			index_of_individual_with_best_training_score_for_all_data = async_error_function.min_error_individual_index();
		}

		training_score_of_individual_with_best_training_score_for_all_data = 0.0 - min_error;

		std::cout << "Group Training Score = " << training_score_of_individual_with_best_training_score_for_all_data << std::endl;

		// Calculate the best individual's test score
		std::vector<int> best_individual_indexes = { index_of_individual_with_best_training_score_for_all_data };
		double error = individual_selection_error_function(best_individual_indexes, test_input_start, test_input_end, 0, false);
		validation_score_of_individual_with_best_training_score_for_all_data = 0.0 - error;

		std::cout << "Group Test Score = " << validation_score_of_individual_with_best_training_score_for_all_data << std::endl;

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
				if (globals::population_agents[individual_index].get_errors()[test_case_index] < test_case_minimum_error[test_case_index])
				{
					test_case_minimum_error[test_case_index] = globals::population_agents[individual_index].get_errors()[test_case_index];
					index_of_best_individual_for_each_test_case[test_case_index] = individual_index;
				}

				if ( (test_case_minimum_error[test_case_index] < 0.0) //std::numeric_limits<double>::max())
					&& (globals::population_agents[individual_index].get_errors()[test_case_index] <= (test_case_minimum_error[test_case_index] + globals::epsilons[test_case_index]))
					)
				{
					set_of_eligible_parents.insert(individual_index);
					globals::population_agents[individual_index].log_elite_test_case(test_case_index);
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

		// Calculate the test error from the best individuals from each test case
		std::cout << "Test error from the best individuals from each test case = ";
		error = individual_selection_error_function(index_of_best_individual_for_each_test_case, test_input_start, test_input_end, 0, false);
		double best_individual_for_each_test_case_group_validation_score = 0.0 - error;
		std::cout << best_individual_for_each_test_case_group_validation_score << std::endl;

		// Calculate the training error for the eligible parents
		std::cout << "Eligible parents training score = ";
		error = individual_selection_error_function(index_of_eligible_parents, training_input_start, training_input_end, 0, false);
		double eligible_parents_training_score = 0.0 - error;
		std::cout << eligible_parents_training_score << std::endl;

		// Calculate the test error for the eligible parents
		std::cout << "Eligible parents test score = ";
		error = individual_selection_error_function(index_of_eligible_parents, test_input_start, test_input_end, 0, false);
		double eligible_parents_validation_score = 0.0 - error;
		std::cout << eligible_parents_validation_score << std::endl;

		// Calculte group training score
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

		for (int individual_index = 0; individual_index < argmap::population_size; individual_index++)
		{
			for (int test_case_index = 0; test_case_index < Number_Of_Test_Cases; test_case_index++)
			{
				if (globals::population_agents[individual_index].get_errors()[test_case_index] <= (test_case_minimum_error[test_case_index] + globals::epsilons[test_case_index]))
				{
					globals::population_agents[individual_index].make_elite();
					number_of_elite_individuals++;
					break;
				}
			}

			if (maximum_number_of_test_cases_for_any_elite_individual < globals::population_agents[individual_index].count_elite_test_cases())
			{
				maximum_number_of_test_cases_for_any_elite_individual = globals::population_agents[individual_index].count_elite_test_cases();
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
		sqlcmd_save_status_report->set_as_string(19, globals::population_agents[index_of_individual_with_best_training_score_for_all_data].get_genome_as_string());

		sqlcmd_save_status_report->execute();

		delete sqlcmd_save_status_report;

		// Save transaction log to file.
		globals::population_agents[index_of_elite_individual_with_maximum_number_test_cases].dump_transactions();
	}

	void pushgp(std::function<double(int, unsigned long, unsigned long)> reproduction_selection_error_function,
		        std::function<double(static std::vector<int> & individual_indexes, static unsigned long input_start, static unsigned long input_end, unsigned int _test_case, bool _record_transactions)> individual_selection_error_function)
	{
		try
		{
			unsigned int generation_number = 1;
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
			Push::LiteralFactory<int> *intLiteralFactory_old = intLiteralFactory;
			Push::LiteralFactory<double> *floatLiteralFactory_old = floatLiteralFactory;
			Push::LiteralFactory<bool> *boolLiteralFactory_old = boolLiteralFactory;
			Push::CodeListFactory* codeListFactory_old = Push::codeListFactory;
			Push::DoRangeClassFactory* doRangeClassFactory_old = Push::doRangeClassFactory;

			// Initialze
			finance::Broker::load_datatable();
			Push::init_static_PushP_instructions();
			Push::init_push();

			// Load population.  Create more if not enough loaded.
			cout << "Create Population Agents" << endl;
			generation_number = get_last_saved_generation_number() + 1;
			agents_created = make_pop_agents(load_pop_agents());
			
			if (agents_created > 0)
				generation_number = 0;

//			cout << "Create Child Agents" << endl;
//			make_child_agents();

			// Save references to the main factories
			intLiteralFactory_old = Push::intLiteralFactory;
			floatLiteralFactory_old = Push::floatLiteralFactory;
			boolLiteralFactory_old = Push::boolLiteralFactory;
			codeListFactory_old = Push::codeListFactory;
			doRangeClassFactory_old = Push::doRangeClassFactory;

			while (!done)
			{
				cout << "Generation " << generation_number << endl;
				save_generation();

				cout << "Compte Errors" << endl;
				compute_errors(reproduction_selection_error_function, argmap::training_start_index, argmap::training_end_index);

				cout << "Number_Of_Test_Cases = " << Number_Of_Test_Cases << endl;

				cout << "Calculate Epsilons" << endl;
				calculate_epsilons_for_epsilon_lexicase();

				cout << "Produce New Offspring" << endl;
				produce_new_offspring();
				
				cout << "Generate status report" << endl;
				generate_status_report(generation_number, 
					individual_selection_error_function, 
					argmap::training_start_index, 
					argmap::training_end_index, 
					argmap::test_start_index, 
					argmap::test_end_index);

				cout << "Install New Generation" << endl;
				install_next_generation();
				generation_number++;
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
			cout << "Standard exception: " << e.what() << endl;
			throw;
		}
		catch (...)
		{
			cout << "Exception occurred" << endl;
			throw;
		}
	}
}