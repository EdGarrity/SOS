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

using namespace std;
using namespace Push;
using namespace pushGP;

namespace pushGP
{
	database::SQLConnection con;

	database::SQLCommand* sqlcmd_delete_indiciduals;
	database::SQLCommand* sqlcmd_insert_new_individual;
	database::SQLCommand* sqlcmd_get_individuals;
	database::SQLCommand* sqlcmd_save_status_report;
	database::SQLCommand* sqlcmd_get_start_index;
	database::SQLCommand* sqlcmd_get_end_index;

	const std::string sqlstmt_delete_individuals("DELETE FROM [SOS].[dbo].[Individuals];");
	const std::string sqlstmt_insert_new_individual("INSERT INTO [dbo].[Individuals] ([Genome]) VALUES (?);");
	const std::string sqlstmt_save_status_report("INSERT INTO [dbo].[ProgressLog]"
		"           ([Generation]"							// 1
		"           ,[Group_TrainingScore]"					// 2
		"           ,[Group_TestScore]"						// 3			
		"           ,[EligibleParents_TrainingScore]"		// 4
		"           ,[EligibleParents_TestScore]"			// 5
		"           ,[EligibleIndividuals_TrainingScore]"	// 6
		"           ,[EligibleIndividuals_TestScore]"		// 7
		"           ,[BestIndividual_TrainingScore]"		// 8
		"           ,[BestIndividual_TestScore]"			// 9
		"           ,[Elite_Size]"							// 10
		"           ,[Elite_TestCases]"						// 11
		"           ,[Opening_Balance]"						// 12
		"           ,[Population_Size])"					// 13
		"     VALUES"
		"           (?,?,?,?,?,?,?,?,?,?,?,?,?)");
			//       1 2 3 4 5 6 7 8 9 0 1 2 3

	//const std::string sqlstmt_get_start_index("SELECT min(Id), min([Date]) AS [StartDate]"
	//	"  FROM [Decision Support System].[dbo].[StockPrices]"
	//	"  WHERE Symbol = 'RIMM'"
	//	"  AND [Date] >= ?");
	//const std::string sqlstmt_get_end_index("SELECT MAX(Id), MAX([Date]) AS [EndDate]"
	//	"  FROM [Decision Support System].[dbo].[StockPrices]"
	//	"  WHERE Symbol = 'RIMM'"
	//	"  AND [Date] <= ?");

	unsigned int load_pop_agents()
	{
		unsigned int n = 0;

		sqlcmd_get_individuals->execute();

		if (sqlcmd_get_individuals->is_result_set())
		{
			while (sqlcmd_get_individuals->fetch_next())
			{
				std::string ind = sqlcmd_get_individuals->get_field_as_string(1);
				Individual individual(ind);
				globals::population_agents[n++] = individual;
			}
		}

		return n;
	}

	void make_pop_agents(int start_)
	{
		for (int n = start_; n < argmap::population_size; n++)
		{
			Individual individual(random_plush_genome());
			globals::population_agents[n] = individual;
		}
	}

	void make_child_agents()
	{
		for (int n = 0; n < argmap::population_size; n++)
		{
			Individual individual = Individual();
			globals::child_agents[n] = individual;
		}
	}

	//double evaluate_individual(std::function<double(Individual&, unsigned long, unsigned long)> reproduction_selection_error_function, Individual & individual, unsigned long input_start, unsigned long input_end)
	//{
	//	return reproduction_selection_error_function(individual, input_start, input_end);
	//}

	void compute_errors(std::function<double(Individual&, unsigned long, unsigned long)> reproduction_selection_error_function, unsigned long input_start, unsigned long input_end)
	{
		double min_error = std::numeric_limits<double>::max();

		for (int n = 0; n < argmap::population_size; n++)
		{
			cout << "  Evaluate Individual " << n;
//			double error = evaluate_individual(reproduction_selection_error_function, globals::population_agents[n], input_start, input_end);
			double error = reproduction_selection_error_function(globals::population_agents[n], input_start, input_end);
			cout << " Min error = " << error << std::endl;
			min_error = min_error < error ? min_error : error;
		}

		cout << "   Min error = " << min_error << std::endl;
	}

	void produce_new_offspring()
	{
		for (unsigned int n = 0; n < argmap::population_size; n++)
			globals::child_agents[n] = breed();
	}

	void install_next_generation()
	{
		for (unsigned int n = 0; n < argmap::population_size; n++)
		{
			globals::population_agents[n] = globals::child_agents[n];
		}
	}

	void save_generation(database::SQLConnection& con)
	{
		// Begin a transaction
		sqlcmd_insert_new_individual->begin_transaction();  //transaction->begin();

		// Delete previously saved generation
		sqlcmd_insert_new_individual->execute(sqlstmt_delete_individuals);
//		cmdInsertNewIndividual->release();
//		cmdDeleteIndividuals->execute();

		// Save new generation
		sqlcmd_insert_new_individual->set_command(sqlstmt_insert_new_individual);

		for (int n = 0; n < argmap::population_size; n++)
		{
			sqlcmd_insert_new_individual->set_as_string(1, globals::population_agents[n]);
			sqlcmd_insert_new_individual->execute();
		}

		// Commit transaction
		sqlcmd_insert_new_individual->commit_transaction();  //transaction->commit();
	}

	void generate_status_report(int generation_, 
		std::function<double(Individual&, unsigned long, unsigned long)> individual_selection_error_function, 
		unsigned int training_input_start, 
		unsigned int training_input_end,
		unsigned int test_input_start,
		unsigned int test_input_end)
	{
		unsigned int n = 0;
		double min_error = std::numeric_limits<double>::max();
		Individual best_individual;

		double group_training_score = 0;
		double group_test_score = 0;

		// Calcuate the group training score
		min_error = std::numeric_limits<double>::max();
		for (auto ind : globals::population_agents)
		{
			std::cout << "Calcuate the group training score for individual #" << ++n << std::endl;

			double error = individual_selection_error_function(ind, training_input_start, training_input_end);

			if (error < min_error)
			{
				min_error = error;
				best_individual = ind;
			}
		}

		group_training_score = 0.0 - min_error;

		std::cout << "Group Training Score = " << group_training_score << std::endl;

		// Calcuate the group test score
		double error = individual_selection_error_function(best_individual, test_input_start, test_input_end);
		group_test_score = 0.0 - error;

		std::cout << "Group Test Score = " << group_test_score << std::endl;

		// Set parameters to save
		sqlcmd_save_status_report->set_as_integer(1, generation_);
		sqlcmd_save_status_report->set_as_float(2, group_training_score);
		sqlcmd_save_status_report->set_as_float(3, group_test_score);

		sqlcmd_save_status_report->set_as_float(4, 0.0);
		sqlcmd_save_status_report->set_as_float(5, 0.0);
		sqlcmd_save_status_report->set_as_float(6, 0.0);
		sqlcmd_save_status_report->set_as_float(7, 0.0);
		sqlcmd_save_status_report->set_as_float(8, 0.0);
		sqlcmd_save_status_report->set_as_float(9, 0.0);
		sqlcmd_save_status_report->set_as_integer(10, 0);
		sqlcmd_save_status_report->set_as_integer(11, 0);

		sqlcmd_save_status_report->set_as_float(12, argmap::opening_balance);
		sqlcmd_save_status_report->set_as_integer(13, argmap::population_size);

		sqlcmd_save_status_report->execute();
	}

	void pushgp(std::function<double(Individual&, unsigned long, unsigned long)> reproduction_selection_error_function,
		        std::function<double(Individual&, unsigned long, unsigned long)> individual_selection_error_function)
	{
		try
		{
			unsigned int generation = 1;
			bool done = false;
			unsigned long training_input_start = argmap::training_start_index;
			unsigned long training_input_end = argmap::training_end_index;
			unsigned long test_input_start = argmap::test_start_index;
			unsigned long test_input_end = argmap::test_end_index;

			con.connect("HOMEOFFICE", "SOS", "MySOS", "MySOS");

			sqlcmd_delete_indiciduals = new database::SQLCommand(&con, sqlstmt_delete_individuals);
			sqlcmd_insert_new_individual = new database::SQLCommand(&con);
			sqlcmd_get_individuals = new database::SQLCommand(&con, "SELECT [Genome] FROM [dbo].[Individuals];");
			sqlcmd_save_status_report = new database::SQLCommand(&con, sqlstmt_save_status_report);
			//sqlcmd_get_start_index = new database::SQLCommand(&con, sqlstmt_get_start_index);
			//sqlcmd_get_end_index = new database::SQLCommand(&con, sqlstmt_get_end_index);

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
			make_pop_agents(load_pop_agents());
			cout << "Create Child Agents" << endl;
			make_child_agents();

			// Save references to the main factories
			intLiteralFactory_old = Push::intLiteralFactory;
			floatLiteralFactory_old = Push::floatLiteralFactory;
			boolLiteralFactory_old = Push::boolLiteralFactory;
			codeListFactory_old = Push::codeListFactory;
			doRangeClassFactory_old = Push::doRangeClassFactory;

			while (!done)
			{
				cout << "Generation " << generation << endl;
				save_generation(con);

				cout << "Compte Errors" << endl;
				compute_errors(reproduction_selection_error_function, training_input_start, training_input_end);

				cout << "Number_Of_Test_Cases = " << Number_Of_Test_Cases << endl;

				cout << "Calculate Epsilons" << endl;
				calculate_epsilons_for_epsilon_lexicase();

				cout << "Produce New Offspring" << endl;
				produce_new_offspring();
				
				cout << "Generate status report" << endl;
				generate_status_report(generation, individual_selection_error_function, training_input_start, training_input_end, test_input_start, test_input_end);

				cout << "Install New Generation" << endl;
				install_next_generation();
				generation++;
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

			delete sqlcmd_delete_indiciduals;
			delete sqlcmd_insert_new_individual;
			delete sqlcmd_get_individuals;
			delete sqlcmd_save_status_report;
			//delete sqlcmd_get_start_index;
			//delete sqlcmd_get_end_index;
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