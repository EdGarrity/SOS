#include "Trader.h"
#include "..\DataStore\CaseData.h"

namespace domain
{
    void Trader::buy(size_t index)
    {
        double price = datastore::case_data.get_stock_price(index);
        double balance = p_account->get_balance();

        if (price <= balance)
        {
			p_account->withdraw(price);
			shares += balance / price;
		}
    }

    void Trader::sell(size_t index)
    {
        double price = datastore::case_data.get_stock_price(index);

        if (shares > 0)
        {
            p_account->deposit(price * shares);
            shares = 0;
        }
    }

    void Trader::execute(size_t index, unsigned long order)
    {
        switch (order)
        {
        case -1:// Buy
            buy(index);
            break;
        case 1: // Sell
            sell(index);
            break;
        }
    }

    double Trader::unrealized_value(size_t index) const
    {
        double price = datastore::case_data.get_stock_price(index);
        double balance = p_account->get_balance();

        if (shares > 0)
        {
            balance += price * shares;
        }

        return balance;
    }
}
