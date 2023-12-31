#include "Account.h"

namespace domain
{
	void Account::withdraw(double amount)
	{
		if (amount > balance)
			throw std::underflow_error("Insufficient funds.");

		balance -= amount;
	}
}
