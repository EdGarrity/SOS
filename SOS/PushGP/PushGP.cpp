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

	database::SQLCommand* cmdDeleteIndividuals;
	database::SQLCommand* cmdInsertNewIndividual;
	database::SQLCommand* cmdGetIndividuals;

	const std::string delete_individuals("DELETE FROM [SOS].[dbo].[Individuals];");
	const std::string insert_new_individual("INSERT INTO [dbo].[Individuals] ([Genome]) VALUES (?);");


	unsigned int load_pop_agents()
	{
		unsigned int n = 0;

		cmdGetIndividuals->execute();

		if (cmdGetIndividuals->is_result_set())
		{
			while (cmdGetIndividuals->fetch_next())
			{
				std::string ind = cmdGetIndividuals->get_field_as_string(1);
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

	double evaluate_individual(Individual & individual, std::function<double(Individual&)> error_function)
	{
		return error_function(individual);
	}

	void compute_errors(std::function<double(Individual&)> error_function)
	{
		double min_error = std::numeric_limits<double>::max();

		for (int n = 0; n < argmap::population_size; n++)
		{
			cout << "  Evaluate Individual " << n;
			double error = evaluate_individual(globals::population_agents[n], error_function);
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
		cmdInsertNewIndividual->begin_transaction();  //transaction->begin();

		// Delete previously saved generation
		cmdInsertNewIndividual->execute(delete_individuals);
//		cmdInsertNewIndividual->release();
//		cmdDeleteIndividuals->execute();

		// Save new generation
		cmdInsertNewIndividual->set_command(insert_new_individual);

		for (int n = 0; n < argmap::population_size; n++)
		{
			cmdInsertNewIndividual->set_as_string(1, globals::population_agents[n]);
			cmdInsertNewIndividual->execute();
		}

		// Commit transaction
		cmdInsertNewIndividual->commit_transaction();  //transaction->commit();
	}

	void pushgp(std::function<double(Individual&)> error_function)
	{
		try
		{
			unsigned int generation = 0;
			bool done = false;

			con.connect("HOMEOFFICE", "SOS", "MySOS", "MySOS");

			cmdDeleteIndividuals = new database::SQLCommand(&con, delete_individuals);
			cmdInsertNewIndividual = new database::SQLCommand(&con);
			cmdGetIndividuals = new database::SQLCommand(&con, "SELECT [Genome] FROM [dbo].[Individuals];");

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
				compute_errors(error_function);

				cout << "Number_Of_Test_Cases = " << Number_Of_Test_Cases << endl;
				cout << "Calculate Epsilons" << endl;
				calculate_epsilons_for_epsilon_lexicase();

				cout << "Produce New Offspring" << endl;
				produce_new_offspring();
				
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

			delete cmdDeleteIndividuals;
			delete cmdInsertNewIndividual;
			delete cmdGetIndividuals;
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