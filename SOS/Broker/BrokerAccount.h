#pragma once

#include "Account.h"

namespace domain
{
	class BrokerAccount
	{
	private:
		//size_t case_number = 0;
		//Account* p_account = nullptr;
		Account account = Account();
		size_t shares = 0;

	public:
		//Trader() : case_number(0), p_account(new Account(10000)), shares(0) {};
		//Trader(size_t case_number, double opening_balance) : case_number(case_number),  p_account(new Account(opening_balance)), shares(0) {};
		//~Trader() { delete p_account; };

		BrokerAccount() : shares(0) { account.set_balance(seed_money); };
		BrokerAccount(double opening_balance) : shares(0) { account.set_balance(opening_balance); };
		~BrokerAccount() {};

		static constexpr double seed_money = 10000.0;

		void initialize(double opening_balance) { shares = 0; account.set_balance(opening_balance); };
		//void buy(size_t index);
		//void sell(size_t index);
		//void execute(size_t index, unsigned long order);
		//double unrealized_gain(size_t index) const;
	};
}