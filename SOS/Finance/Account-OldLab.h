#ifndef _ACCOUNT__H__
#define _ACCOUNT__H__

namespace finance
{
	class Account
	{
	private:
		double cash;
		_ULonglong quantity;

	public:
		Account() : cash (0.0) , quantity (0) {}
		Account(double openingBalance);

		double Cash();
		_ULonglong Quantity();

		void Purchase(_ULonglong quantityToPurchase, double totalCost);
		void Sell(_ULonglong quantityToSell, double totalProfit, double taxRate);
		void Reset(double openingBalance);
	};
}

#endif
