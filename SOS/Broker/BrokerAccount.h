#pragma once

#include <cstddef>
#include "Account.h"
#include "..\DataStore\FinancialData.h"

namespace domain
{
	class BrokerAccount
	{
	private:
		datastore::FinancialData::FinancialInstrumentType financial_instrument_type = datastore::FinancialData::FinancialInstrumentType::Primary;
		Account account = Account();
		size_t shares = 0;

	public:
		BrokerAccount() : shares(0) { account.set_balance(seed_money); };
		BrokerAccount(datastore::FinancialData::FinancialInstrumentType _financial_instrument_type, double opening_balance) 
			: shares(0), financial_instrument_type(_financial_instrument_type) 
		{ 
			account.set_balance(opening_balance); 
		};
		~BrokerAccount() {};

		static constexpr double seed_money = 10000.0;

		void initialize(double opening_balance) { shares = 0; account.set_balance(opening_balance); };
		void buy(size_t index);
		void sell(size_t index);
		void execute(size_t index, unsigned long order);
		void trace_execute(size_t index, unsigned long order);
		double unrealized_gain(size_t index) const;
	};
}