#pragma once

#include "Account.h"

namespace domain
{
	class Trader
	{
	private:
		size_t case_number = 0;
		Account* p_account = nullptr;
		size_t shares = 0;

	public:
		Trader() : case_number(0), p_account(new Account(10000)), shares(0) {};
		Trader(size_t case_number, double opening_balance) : case_number(case_number),  p_account(new Account(opening_balance)), shares(0) {};
		~Trader() { delete p_account; };

		void buy(size_t index);
		void sell(size_t index);
		void execute(size_t index, unsigned long order);
		double unrealized_value(size_t index) const;
	};
}