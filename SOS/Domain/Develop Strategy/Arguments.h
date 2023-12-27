	#pragma once
#include <vector>
#include <string>

namespace domain
{
	namespace argmap
	{
		// Diagnostic Level definitions
		const int diagnostic_level_1 = 1;
		const int diagnostic_level_2 = 2;
		const int diagnostic_level_3 = 3;
		const int diagnostic_level_4 = 4;
		const int diagnostic_level_5 = 5;
		const int diagnostic_level_6 = 6;
		const int diagnostic_level_7 = 7;
		const int diagnostic_level_8 = 8;
		const int diagnostic_level_9 = 9;

		// Diagnostic Level
		const int diagnostic_level = diagnostic_level_9;


		// Specify which financial instruments to target
		const std::string financial_instrument = "AAPL";
		const std::string financial_index = "FXAIX";
		const std::string financial_data_start_date = "2020-01-01";
		const std::string financial_data_end_date = "2021-12-31";


		//	Select which algorithm to use
		enum class AlgorithmSelection { learn_from_examples, strategy_development };
		const AlgorithmSelection algorithm_selection = AlgorithmSelection::strategy_development;

		// Define length of the training and test cases
		const unsigned long training_case_length = 252; // 252 trading days in a year
		const unsigned long test_case_length = 252; // 252 trading days in a year

		// Default maximum size of the stack buffer.  Must be greater than max_points
		const unsigned long maximum_stack_size = 2000;

		const unsigned long maximum_stack_dept = 100;

		const double PI = 3.141592653589793238463;

		//	;; ----------------------------------------
		//	;; System arguments
		//	;; ----------------------------------------

		//	:use - single - thread false
		//	;; When true, will only use a single thread.
		const bool use_multithreading = true;
		const bool use_PPL = false;
		const unsigned long number_of_cores_to_reserve = 4;
		const unsigned long max_threads = 8;
		const unsigned long thread_chunk_size = 1; // 100;  // Number of threads per chunk.

		// CPU temperature monitoring
		const std::string GetCpuTemperatureCmd = "C:\\Users\\edgar\\OneDrive\\Projects\\Source\\Repos\\EdGarrity\\SOS\\Debug\\GetCpuTemperature.exe";

		const double cool_temperature = 40;
		const double hot_temperature = 60;
		const long cool_down_minutes = 1;

		const unsigned long percent_memory_cap = 90;

		//	:random - seed(random / generate - mersennetwister - seed)
		//	;; The seed for the random number generator.

		//	:run - uuid nil
		//	;; This will be set to a new type 4 pseudorandom UUID on every run.

		//	;; ----------------------------------------
		//	;; Standard GP arguments
		//	;; ----------------------------------------

		//	:error - function(fn[p] '(0))
		//		;; Function that takes a program and returns a list of errors.

		//		:error - threshold 0
		//		;; Pushgp will stop and return the best program if its total error
		//		;; is <= error - threshold.

		//		:atom - generators(into @registered-instructions
		//		(list
		//		(fn[](lrand - int 100))
		//			(fn[](lrand))))
		//		;; The instructions that pushgp will use in random code.

		// Number of individuals in the population.
		const size_t population_size = 100; // 1000; // 20000; // 200'000;

		// The maximum number of generations to run GP.
		const unsigned long max_generations_in_one_session = 300;

		//		;; The limit for the number of point(instruction) evaluations to
		//		;; execute during the run.
		const unsigned long max_point_evaluations = 1000; //10000;  //10,000,000; //INT_MAX; // 100000; // 10e100

		//		:genome - representation : plush
		//		;; The representation for the genomes used for initialiation and inheritance.
		//		;; Options include : plush and : plushy

	// Maximum size of push programs and push code, as counted by points
	// in the program. 1 / 4 this limit is used as the limit for sizes of
	// Plush genomes.
		const unsigned long max_points = maximum_stack_size - 1;

		// Maximum size of initial Plush genomes in generation 0. Keep in mind
		// that genome lengths will otherwise be limited by 1 / 4 of :max - points.
		const unsigned long max_genome_size_in_initial_program = max_points / 4;

		// Number of Available Training Cases
		//const unsigned long number_of_training_cases = 5316696; // '2020-01-01' to '2021-12-31'
		const unsigned long number_of_training_cases = 1000; // 504; // '2020-01-01' to '2021-12-31'

		// Number of Available Test Cases
		const unsigned long number_of_test_cases = 10;

		// Minimum length of an example case
		const unsigned long example_case_min_length = 2;

		// Maximum length of an example case
		const unsigned long example_case_max_length = 4;

		// Upper range of values in the example case
		const unsigned long example_case_upper_range = 1000;

		// Threshold fpr cutting the merge between any two clusters.  Used to count the number of clusters that differ on at least the threshold amount of the training cases
		const double cluster_break_threshold = 0.1;

		// Dynamic instruction set - Can Push programs add and delete instructions from the Piush Instruction set?
		const bool static_instruction_set = true;

		//// Date range for training
		//const long int training_start_index = 0;
		//const long int training_end_index = 9;

		//// Date range for testing
		//const long int test_start_index = 2508;	// 2010-01-04
		//const long int test_end_index = 3765;	// 2014-12-31

		//const long int last_data_index = test_end_index;

		//		: evalpush - limit 150
			//		;; The number of Push instructions that can be evaluated before stopping
			//		;; evaluation.

			//		:evalpush - time - limit 0
			//		;; The time in nanoseconds that a program can evaluate before stopping,
			//		;; 0 means no time limit.

			//		:reuse - errors true
			//		;; When true, children produced through direct reproduction will not be
			//		;; re - evaluated but will have the error vector of their parent.

			//		;; ----------------------------------------
			//		;; Genetic operator probabilities
			//		;; ----------------------------------------

			//		:genetic - operator-probabilities{ :reproduction 0.0
			//										 : alternation 0.7
			//										 : uniform - mutation 0.1
			//										 : uniform - instruction - mutation 0.0
			//										 : uniform - integer - mutation 0.0
			//										 : uniform - float - mutation 0.0
			//										 : uniform - tag - mutation 0.0
			//										 : uniform - string - mutation 0.0
			//										 : uniform - boolean - mutation 0.0
			//										 ; Similar to the old ULTRA operator:
			//										 [:alternation:uniform - mutation] 0.2
			//										 : uniform - close - mutation 0.0
			//										 : uniform - silence - mutation 0.0
			//										 : uniform - crossover 0.0
			//										 : two - point - crossover 0.0
			//										 ; A hill - climbing version of uniform - silence - mutation:
			//										 [:make - next - operator-revertable : uniform - silence - mutation] 0.0
			//										 : autoconstruction 0.0
			//										 : uniform - deletion 0.0
			//										 : uniform - addition 0.0
			//										 : uniform - addition - and-deletion 0.0
			//										 : uniform - combination - and-deletion 0.0
			//										 : genesis 0.0
			//										 : gene - selection 0.0
			//		}
			//;; The map supplied to : genetic - operator-probabilities should contain genetic operators
			//	;; that sum to 1.0.All available genetic operators are defined in clojush.pushgp.breed.
			//	;; Along with single operators, pipelines(vectors) containing multiple operators are
			//	;; also allowed, where each operator is applied to the child of the previous operator,
			//	;; along with newly selecting individuals where necessary.If an operator is preceeded by
			//	;; :make - next - operator-revertable, it will only keep the child if it is at least as good
			//	;; as its(first) parent on every test case.

			//	;; ----------------------------------------
			//	;; Arguments related to genetic operators
			//	;; ----------------------------------------

		// Genetic Operator Probabilities (must add up to 100%) - Not used if SA enabled
		const double probability_of_alternation = 0.80;
		const double probability_of_mutation = 0.10;
		const double probability_of_cloaning = 0.10;

		// Genetic Operator SA Probabilities
		const double heat_up_rate = 0.10;
		const double cool_down_rate = 0.10;
		const double stalled_delta = 0.005;
		const int stalled_count_trigger = 20;
		const int cool_down_period = 25;

		const double probability_of_alternation_at_minimum_tempareture = 0.70;
		const double probability_of_alternation_at_mid_tempareture = 0.60;
		const double probability_of_alternation_at_maximum_tempareture = 0.50;
		const double probability_of_alternation_slope_1 = probability_of_alternation_at_mid_tempareture - probability_of_alternation_at_minimum_tempareture;
		const double probability_of_alternation_slope_2 = probability_of_alternation_at_maximum_tempareture - probability_of_alternation_at_mid_tempareture;

		const double probability_of_alternation_elite_at_minimum_tempareture = 0.70;
		const double probability_of_alternation_elite_at_mid_tempareture = 0.45;
		const double probability_of_alternation_elite_at_maximum_tempareture = 0.0;
		const double probability_of_alternation_elite_slope_1 = probability_of_alternation_elite_at_mid_tempareture - probability_of_alternation_elite_at_minimum_tempareture;
		const double probability_of_alternation_elite_slope_2 = probability_of_alternation_elite_at_maximum_tempareture - probability_of_alternation_elite_at_mid_tempareture;

		const double probability_of_mutation_at_minimum_tempareture = 0.10;
		const double probability_of_mutation_at_mid_tempareture = 0.20;
		const double probability_of_mutation_at_maximum_tempareture = 0.50;
		const double probability_of_mutation_slope_1 = probability_of_mutation_at_mid_tempareture - probability_of_mutation_at_minimum_tempareture;
		const double probability_of_mutation_slope_2 = probability_of_mutation_at_maximum_tempareture - probability_of_mutation_at_mid_tempareture;

		const double probability_of_cloaning_at_minimum_tempareture = 0.10;
		const double probability_of_cloaning_at_mid_tempareture = 0.10;
		const double probability_of_cloaning_at_maximum_tempareture = 0.10;
		const double probability_of_cloaning_slope_1 = probability_of_cloaning_at_mid_tempareture - probability_of_cloaning_at_minimum_tempareture;
		const double probability_of_cloaning_slope_2 = probability_of_cloaning_at_maximum_tempareture - probability_of_cloaning_at_mid_tempareture;

		const double probability_of_regeneraton_at_minimum_tempareture = 0.00;
		const double probability_of_regeneraton_at_mid_tempareture = 0.00;
		const double probability_of_regeneraton_at_maximum_tempareture = 1.0;
		const double probability_of_regeneraton_slope_1 = probability_of_regeneraton_at_mid_tempareture - probability_of_regeneraton_at_minimum_tempareture;
		const double probability_of_regeneraton_slope_2 = probability_of_regeneraton_at_maximum_tempareture - probability_of_regeneraton_at_mid_tempareture;

		// The probability of generating a constant Plush atom
		const double probability_of_generating_a_constant_Plush_atom = 0.10;

		// The probability of generating an input Plush atom
		const double probability_of_generating_a_io_atom = 0.20;

		// When generating a constant, the range of possible values the constant can be
		const double float_min = ((std::numeric_limits<double>::lowest)() / 2.0);
		const double float_max = ((std::numeric_limits<double>::max)() / 2.0);
		const long int_min = (std::numeric_limits<int>::lowest)();
		const long int_max = (std::numeric_limits<int>::max)();

		// When using alternation, the probability of alternating between the parents when moving
		// to the next gene.
		const double alternation_rate = 0.20;

		// When using alternation, the standard deviation of how far alternation may jump between
		// indices when switching between parents.
		const unsigned long alignment_deviation = 10;

		// The probability of each token being mutated during uniform mutation.
		const double uniform_mutation_rate = 0.20;

		//	The probability of mutating a constant instead of simply replacing the token
		//	with a random instruction during uniform mutation.
		const double uniform_mutation_constant_tweak_rate = 0.50;

		//	The standard deviation used when tweaking float/int constants with Gaussian noise.
		const double uniform_mutation_float_int_gaussian_standard_deviation = 1.0;

		//	:uniform - mutation - int - gaussian - standard - deviation 1
		//	;; The standard deviation used when tweaking integer constants with Gaussian noise.

		//	The probability of each character being changed when doing string constant tweaking.
		const double uniform_mutation_string_char_change_rate = 0.10;

		//	:uniform - mutation - tag - gaussian - standard - deviation 100
		//	;; The standard deviation used when tweaking tag locations with Gaussian noise.

		//	:uniform - close - mutation - rate 0.1
		//	;; The probability of each : close being incremented or decremented during uniform close
		//	;; mutation.

		//	:close - increment - rate 0.2
		//	;; The probability of making an increment change to : close during uniform close mutation,
		//	;; as opposed to a decrement change.

		//	:uniform - deletion - rate 0.01
		//	;; The probability that any instruction will be deleted during uniform deletion.

		//	:uniform - addition - rate 0.01
		//	;; The probability that any instruction will have a new one added before or after it during
		//	;; uniform addition.

		//	:uniform - addition - and-deletion - rate 0.01
		//	;; The probability, per gene, for additions in the first phase, and deletions in the second
		//	;; phase(calculated for size - neutrality), of uniform - addition - and-deletion.

		//	:uniform - combination - rate 0.01
		//	;; The probability, per gene, for combinations during uniform - combination

		//	:uniform - combination - and-deletion - rate 0.01
		//	;; The probability, per gene, for combinations in the first phase, and deletions in
		//	;; the second phase(calculated for size - neutrality), of uniform - combination - and-deletion.

		//	:uniform - silence - mutation - rate 0.1
		//	;; The probability of each : silent being switched during uniform silent mutation.

		//	: replace - child - that - exceeds - size - limit - with : random
		//	;; When a child is produced that exceeds the size limit of(max - points / 4), this is
		//	;; used to determine what program to return.Options include : parent, : empty, : random,
		//	;; :truncate.

		//	: parent - reversion - probability 1.0
		//	;; The probability of a child being reverted to its parent by a genetic operator that
		//	;; has been made revertable, if the child is not as good as the parent on at least one
		//	;; test case.

		//	:tag - enrichment 0
		//	;; The number of extra copies of tag - related instructions that will be included in
		//	;; the atom - generators.

		//	:tag - enrichment - types[:integer:boolean:exec:float : char : string : code]
		//	;; The types for tag - related instructions that will be included in the atom - generators
		//	;; when:tag - enrichment is greater than 0.

		//	: gene - selection - rate 1.0
		//	;; When using gene - selection, the probability of selecting a new parent when moving
		//	;; to the next gene.

		//	:autoconstructive false
		//	;; If true, then :genetic - operator-probabilities will be{ :autoconstruction 1.0 },
		//	;; :epigenetic - markers will be[:close:silent], and : atom - generators will include
		//	;; instructions specified via : autoconstructive - genome - instructions, and other settings
		//	;; specified in augment - for - autoconstruction.Also sets
		//	;; :replace - child - that - exceeds - size - limit - with to : empty.Also, empty - genome individuals
		//	;; will not be selected as parents.You will probably also want to provide a high value
		//	;; for :max - generations.If : autoconstructive is : revertable, rather than true, then
		//	;; :genetic - operator-probabilities will be{ [:make - next - operator-revertable
		//	;; :autoconstruction] 1.0 }.

		//	: autoconstructive - diversification - test : gecco2016
		//	;; Specifies the diversification test for autoconstruction.The default value of
		//	;; :gecco2106, which uses the diversification test used in the work for the
		//	;; paper "Evolution Evolves with Autoconstruction" in the ECADA workshop at GECCO 2016.
		//	;; For other options and their effects, see the definition of diversifying ? in
		//	;; genetic - operators.clj.

		//	:autoconstructive - genome - instructions : all
		//	;; Specifies the genome instructions to use for autoconstruction.The default value of
		//	;; :all will use all genome instructions.See load - push - argmap in this file(args.clj)
		//	;; for other options.

		//	:autoconstructive - clone - probability 0.0
		//	;; Specifies the probability that a clone will be produced rather than the result of
		//	;; actual autoconstruction, when :autoconstructive is true.

		//	: autoconstructive - decay 0.0
		//	;; The rate for random gene deletions after autoconstruction.

		//	:autoconstructive - clone - decay : same
		//	;; The rate for random gene deletions after cloning in autoconstruction.If this is
		//	;; :same then the value for : autoconstructive - decay is used.

		//	: autoconstructive - parent - decay 0.0
		//	;; The rate for random gene deletions in parent genomes used for autoconstruction.

		//	:autoconstructive - diffmeans - children 10
		//	;; When using :autoconstructive - diversification - test : diffmeans - diversifying ? , specifies
		//	;; how many children of each child to generate and test.See genetic - operators.clj.

		//	:autoconstructive - si - children 8
		//	;; When using a "size and instruction" diversification test, specifies how many
		//	;; children to generate and test.See genetic - operators.clj.

		//	:autoconstructive - integer - rand - enrichment 0
		//	;; The number of extra instances of autoconstructive_integer_rand to include in
		//	;; :atom - generators for autoconstruction.If negative then autoconstructive_integer_rand
		//	;; will not be in : atom - generators at all.

		//	: autoconstructive - boolean - rand - enrichment 0
		//	;; The number of extra instances of autoconstructive_boolean_rand to include in
		//	;; :atom - generators for autoconstruction.If negative then autoconstructive_boolean_rand
		//	;; will not be in : atom - generators at all.

		//	: autoconstructive - code - rand - atom - enrichment 0
		//	;; The number of extra instances of autoconstructive_code_rand_atom to include in
		//	;; :atom - generators for autoconstruction.If negative then autoconstructive_code_rand_atom
		//	;; will not be in : atom - generators at all.

		//	: autoconstructive - tag - types[:integer:boolean:exec:float : char : string : code]
		//	;; The types for tag - related instructions that will be included in the atom - generators
		//	;; when:autoconstructive is true.

		//	: autoconstructive - environments false
		//	;; If true, then :environment is included in the types for which instructions are
		//	;; included for autoconstruction.

		//	;; ----------------------------------------
		//	;; Epignenetics
		//	;; ----------------------------------------

		//	:epigenetic - markers[:close]
		//	;; A vector of the epigenetic markers that should be used in the individuals.
		//	;; Implemented options include : : close, : silent

		// A vector of the probabilities for the number of parens ending at that position.
		const std::vector<double> close_parens_probabilities = { 0.772, 0.206, 0.021, 0.001 };

		//	:silent - instruction - probability 0.2
		//	;; If:silent is used as an epigenetic - marker, this is the probability of random
		//	;; instructions having : silent be true.

		//	: track - instruction - maps false
		//	;; If true, each Plush instruction map will have a UUID attached to it.If the
		//	;; gene has a "parent gene", it will also have the UUID of its parent.

		//	;; ----------------------------------------
		//	;; Arguments related to Plushy representaion
		//	;; ----------------------------------------

		//	:plushy - close - probability : automatic
		//	;; Probability of choosing a : close(meta - ) instruction when generating
		//	;; Plushy genomes
		//	;; Can either be a probability in[0, 1], or if :automatic, will be automatically
		//	;; calculated based on the number of open parentheses generated by the instructions
		//	;; in atom - generators

		//	;; ----------------------------------------
		//	;; Arguments related to parent selection
		//	;; ----------------------------------------

		// Incest probabilities
		const double probability_of_sibling_incest = 0.10;
		const double probability_of_first_cousin_incest = 0.20;
		const double probability_of_second_cousin_incest = 0.30;

		// Which parents have a chance to survive to the next generation.
		//const unsigned int error_ratio_cap_for_retaining_parents = 0.2;

		//	:parent - selection : lexicase
		//	;; The parent selection method.Options include : epsilon_lexicase, downsampled_lexicase
		enum class PerentSelection { epsilon_lexicase, downsampled_lexicase };
		const PerentSelection parent_selection = PerentSelection::epsilon_lexicase;

		//:downsample - factor 1
		//	;; Determines the proportion of cases to use when using downsampled lexicase.
		//	;; When set to 1, has no effect.Should be in the range(0, 1].
		const double downsample_factor = 1.0;

		//	: epsilon - lexicase - epsilon nil
		//	;; When parent - selection is : epsilon - lexicase,
		//	;; the value for epsilon.If nil, automatic epsilon lexicase selection will be used.

		//	:epsilon - lexicase - probability 1
		//	;; The probability that each filtering step in epsilon lexicase selection will allow
		//	;; candidates with errors within epsilon of the best to survive, rather than just
		//	;; the best.

		//	:random - threshold - lexicase - probability 1
		//	;; The probability that each filtering step in random threshold lexicase selection will
		//	;; allow candidates with errors equal to or better than a randomly chosen threshold to
		//	;; survive, rather than just the best.

		//	:random - toggle - lexicase - probability 1
		//	;; The probability that each filtering step in random toggle lexicase selection will
		//	;; allow just the best to survive, rather than all individuals in the pool.

		//	:randomly - truncated - lexicase - probability 1
		//	;; The probability that an application of randomly - truncated - lexicase - selection
		//	;; will consider only a random subset of the test cases, rather than all of them.

		//	:lexicase - leakage 0.1
		//	;; If using leaky lexicase selection, the probability that a selection event will return
		//	;; a random(tourny 1) individual from the entire population.

		//	:lexicase - slippage 0
		//	;; If using lexicase, leaky lexicase, epsilon lexicase, or random threshold lexicase
		//	;; selection, the probability that each step of the lexicase selection process will
		//	;; "slip" and return a random candidate from the current pool, rather than continuing
		//	;; to filter the pool.

		//	:sort - meta - errors - for - lexicase : random
		//	;; If using lexicase selection, determines how meta - errors will be sorted among
		//	;; the actual errors.Options are : random(errors and meta - errors are shuffled
		//		;; together), : first(meta - errors come first), or : last(meta - errors come last).

		//	: tournament - size 7
		//	;; If using tournament selection, the size of the tournaments.

		//	:total - error - method : sum
		//	;; The method used to compute total error.Options include : sum(standard), : hah
		//	;; (historically - assessed hardness), :rmse(root mean squared error), and : ifs
		//	;; (implicit fitness sharing).

		//	:normalization:none
		//	;; The method used to normalize the errors to the range[0, 1], with 0 being best.Options
		//	;; include:none(no normalization), : divide - by - max - error(divides by value of argument
		//		;; :max - error), : e - over - e - plus - 1 (e / (e + 1) = 1 - 1 / (e + 1))

		//	: max - error 1000
		//	;; If:normalization is set to : divide - by - max - error, will use this number for
		//	;; normalization.

		//	:meta - error - categories[]
		//	;; A vector containing meta - error categories that can be used for parent selection,
		//	;; but that do not affect total error or the determination of whether an individual
		//	;; is considered to be a solution.Each meta - error - category should either be a function
		//	;; (which must be namespace - qualified if provided in a command - line argument) or a
		//	;; keyword corresponding to a pre - defined meta - error function.In either case the
		//	;; function should take an individual, an evaluated population, and an argmap, and
		//	;; it should return a numeric meta error value or collection of values, for which
		//	;; lower is interpreted as better.For keyword : foo, the corresponding meta - error
		//	;; function will be clojush.meta - errors / foo - meta - error.See clojush.meta - errors for
		//	;; the current options for pre - defined meta - error functions.

		//	:improvement - discount 0.5
		//	;; The factor by successively older improvements are discounted when calculating
		//	;; improvement - related meta - errors.

		//	:decimation - ratio 1;; If >= 1, does nothing.Otherwise, is the percent of the population
		//	;; size that is retained before breeding.If 0 < decimation - ratio < 1, decimation
		//	;; tournaments will be used to reduce the population to size(*population - size
		//		;; decimation - ratio) before breeding.

		//	:decimation - tournament - size 2
		//	;; Size of the decimation tournaments.

		//	:print - selection - counts false
		//	;; If true, keeps track of and prints the number of times each individual was selected
		//	;; to be a parent

		//	: self - mate - avoidance - limit 0
		//	;; If non - zero, then when multiple parents are required for a genetic operator, an
		//	;; effort will be made to select parents not equal to the first parent.The value
		//	;; of this parameter is the number of re - selections that will be performed to try
		//	;; to find a different parent, before using the same parent if the limit is exceeded.

		//	:age - mediated - parent - selection false
		//	;; If truthy, should be a vector of[pmin pmax].In this case, then with probability
		//	;; pmin, parent selection will consider only individuals with the minimum age in
		//	;; the population; with probability pmax, all individuals will be considered; with
		//	;; probability(-1.0 pmin pmax) an age cutoff will be chosen uniformly from
		//	;; those present in the population, and only individuals with the cutoff age or
		//	;; lower will be considered.
		//	;;
		//;; NOTE: It doesn't make any sense to use this unless you have multiple ages in the
		//	;; population, as you migh have, for example, from using the genesis operator or
		//	;; autoconstruction.

		//	:age - combining - function : average
		//	;; For genetic operators that involve multiple parents, the function used to combine
		//	;; the incremented ages of the parents to produce the age of the child.

		//	:random - screen false
		//	;; If truthy, should be a map with values for :criterion, : probability and possibly
		//	;; :reversible.In this case, then with probability : probability, each parent
		//	;; selection event will consider only individuals with : grain - size equal to or less
		//	;; than a : grain - size chosen randomly from those present in the population.The
		//	;; :criterion(see genetic - operators.clj for options) determines how : grain - size is
		//	;; computed for an individual when it is created.If :reversible is truthy, then the
		//	;; screen will be applied in reverse with probability 1 / 2, causing parent selection
		//	;; to consider only individuals with : grain - size equal to or GREATER than the
		//	;; chosen:grain - size.

		//	: novelty - distance - metric : euclidean
		//	;; When using novelty, the distance metric between two behavior vectors
		//	;; Options: : manhattan, : euclidean

		//	: individuals - for - novelty - archive - per - generation 0
		//	;; The number of individuals to add to the novelty archive per generation, if
		//	;; using novelty search for parent selection or novelty as a meta - error.Default
		//	;; of 0 means archive won't be maintained. Novelty GP paper uses one individual
		//	;; every - other generation for 1000 generations; as such, a value of 1 seems like
		//	;; a reasonable place to start.

		//	:novelty - number - of - neighbors - k 25
		//	;; The number of neighbors to consider when calculating the sparseness with
		//	;; regard to the nearest neighbors.Paper claims it is "robust to modest variation."

		//	;; ----------------------------------------
		//	;; Arguments related to the Push interpreter
		//	;; ----------------------------------------

		//	:pop - when - tagging true
		//	;; When true, tagging instructions will pop the exec stack when tagging; otherwise, the
		//	;; exec stack is not popped.

		//	:tag - limit 10000
		//	;; The size of the tag space.

		//	:top - level - push - code false
		//	;; When true, run - push will push the program's code onto the code stack prior to running.

		//	:top - level - pop - code false
		//	;; When true, run - push will pop the code stack after running the program.

		//	;; ----------------------------------------
		//	;; Arguments related to generational and final reports
		//	;; ----------------------------------------

		//	:report - simplifications 100
		//	;; The number of simplification steps that will happen during report simplifications

		//	: final - report - simplifications 1000
		//	;; The number of simplification steps that will happen during final report
		//	;; simplifications.

		//	:problem - specific - initial - report default - problem - specific - initial - report
		//	;; A function can be called to provide a problem - specific initial report, which happens
		//	;; before the normal initial report is printed.

		//	:problem - specific - report default - problem - specific - report
		//	;; A function can be called to provide a problem - specific report, which happens before
		//	;; the normal generational report is printed.

		//	:return-simplified - on - failure false
		//	;; When true, will simplify the best indivual and return it, even if the error threshold
		//	;; has not been reached.This will make failures return the same as successes.

		//	:print - errors true
		//	;; When true, prints the error vector of the best individual.

		//	:print - history false
		//	;; When true, prints the history of the best individual's ancestors' total errors.

		//	:print - timings false
		//	;; If true, report prints how long different parts of evolution have taken during the
		//	;; current run.

		//	:print - error - frequencies - by - case false
		//	; If true, print reports of error frequencies by case each generation.

		//	:print - cosmos - data false
		//	;; If true, report prints COSMOS data each generation.

		//	:maintain - ancestors false
		//	;; If true, save all ancestors in each individual(costly).

		//	:print - ancestors - of - solution false
		//	;; If true, final report prints the ancestors of the solution.Requires
		//	;; :maintain - ancestors to be true.

		//	: print - homology - data false
		//	;; If true, prints the homology statistics.

		//	:exit - on - success true
		//	;; When true, will exit the run when there is an individual with a zero - error vector

		//	: visualize false
		//	;; When true, graphics will be displayed to indicate system progress.
		//	;; See the comments in src / clojush / pushgp / visualize.clj

		//	;; ----------------------------------------
		//	;; Arguments related to printing JSON, EDN, CSV, and remote recording
		//	;; ----------------------------------------

		//	:print - csv - logs false
		//	;; Prints a CSV log of the population each generation.

		//	:print - edn - logs false
		//	;; Prints an EDN log of the run.

		//	:print - json - logs false
		//	;; Prints a JSON log of the population each generation.

		//	:csv - log - filename "log.csv"
		//	;; The file to print CSV log to.

		//	:edn - log - filename "log.edn"
		//	;; The file to print EDN log to.

		//	:json - log - filename "log.json"
		//	;; The file to print JSON log to.

		//	:csv - columns[:generation:location:total - error : push - program - size]
		//	;; The columns to include in a printed CSV beyond the generation and individual.Options
		//	;; include: [:generation:location:parent - uuids : genetic - operators : push - program - size
		//	;; :plush - genome - size : push - program : plush - genome : genome - closes : push - paren - locations
		//	;; :total - error : test - case-errors]

		//	: edn - keys[:uuid:parent - uuids : genetic - operators : program : genome : total - error : errors]
		//	;; Keys from clojush.individual.individual that should be included.

		//	:edn - additional - keys[:generation:location]
		//	;; Additional information to include in the edn - printout.Available options are
		//	;; [:generation:location:push - program - size : plush - genome - size].

		//	:log - fitnesses - for - all - cases false
		//	;; If true, the CSV and JSON logs will include the fitnesses of each individual on every
		//	;; test case.

		//	:json - log - program - strings false
		//	;; If true, JSON logs will include program strings for each individual.

		//	:record - host nil
		//	;; Should be in the format "<hostname>:<port>"
		//	;; If set, will send logs of each run to a server running on this
		//	;; host
		//	:label nil
		//	;; If set, will send this in the configuration of the run, to the
		//	;; external record

		//	;; ----------------------------------------
		//	;; Arguments related to progress reports and recording
		//	;; ----------------------------------------

		// Network name of a server running an instance of MicrosoftSQL Server. If there are multiple instances of SQL Server running on the computer, in order to 
		// connect to a specific instance of SQL Server the value init_datasource is specified as \\ServerName\InstanceName. The escape sequence \\ is used for 
		// backslash itself
//		const std::string db_init_datasource = "HOMEOFFICE";
		//const std::string db_init_datasource = "(local)";
		//const std::string db_init_datasource = "EGARRITY-LT";
		const std::string db_init_datasource = "192.168.1.42";

		// Name of an existing SQL Server database to which to connect.
		const std::string db_init_catalog = "SOS";

		// SQL Server login. This property is used when SQL Server Authentication is selected for authorizing access to a SQL Server database.
		const std::string db_user_id = "MySOS";

		// Password assigned to a SQL Server login. This property is used when SQL Server Authentication is selected for authorizing access to a SQL Server database.
		const std::string db_user_password = "MySOS";
	}
}
