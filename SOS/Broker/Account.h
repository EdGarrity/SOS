#pragma once

#include <stdexcept>

namespace domain
{
	class Account
	{
	private:
		double balance = 0;

	public:
		Account() : balance(0) {};
		Account(double opening_balance) : balance(opening_balance) {};

		double get_balance() const { return balance; };
		void deposit(double amount) { balance += amount; };
		void withdraw(double amount);
	};
}