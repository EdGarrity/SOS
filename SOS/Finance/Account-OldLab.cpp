#include <yvals.h>
//#include <Windows.h>
#include "Account.h"

namespace finance
{
	Account::Account(double openingBalance)
	{
		cash = openingBalance;
		quantity = 0;
	}

	double Account::Cash()
	{
		return cash;
	}

	_ULonglong Account::Quantity()
	{
		return quantity;
	}

	void Account::Purchase(_ULonglong quantityToPurchase, double totalCost)
	{
		if (cash >= totalCost)
		{
			quantity += quantityToPurchase;
			cash -= totalCost;
		}
	}

	void Account::Sell(_ULonglong quantityToSell, double totalProfit, double taxRate)
	{
		if ((quantity >= quantityToSell) && (quantity > 0))
		{
			quantity -= quantityToSell;
			cash += totalProfit * (1.0 - taxRate);
		}
	}

	void Account::Reset(double openingBalance)
	{
		cash = openingBalance;
		quantity = 0;
	}
}
