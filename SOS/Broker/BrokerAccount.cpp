#include "BrokerAccount.h"
#include "..\DataStore\TestData.h"

namespace domain
{
    void BrokerAccount::buy(size_t index)
    {
        double price = datastore::test_data.get_stock_price(index);
        //double balance = p_account->get_balance();
        double balance = account.get_balance();

        if (price <= balance)
        {
			//p_account->withdraw(price);
            account.withdraw(price);
            shares += balance / price;
		}
    }

    void BrokerAccount::sell(size_t index)
    {
        double price = datastore::test_data.get_stock_price(index);

        if (shares > 0)
        {
            //p_account->deposit(price * shares);
            account.deposit(price * shares);
            shares = 0;
        }
    }

    void BrokerAccount::execute(size_t index, unsigned long order)
    {
        if (order < 0)
            buy(index);

        else if (order > 0)
            sell(index);
    }

    double BrokerAccount::unrealized_value(size_t index) const
    {
        double price = datastore::test_data.get_stock_price(index);
        //double balance = p_account->get_balance();
        double balance = account.get_balance();

        if (shares > 0)
        {
            balance += price * shares;
        }

        return balance;
    }
}
