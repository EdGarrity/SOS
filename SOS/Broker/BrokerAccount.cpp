#include "BrokerAccount.h"
#include "..\Utilities\Debug.h"

namespace domain
{
    void BrokerAccount::buy(size_t index)
    {
        double price = 0;

        if (financial_instrument_type == datastore::FinancialData::FinancialInstrumentType::Primary)
            price = datastore::financial_data.get_primary_stock_price(index);
        else
            price = datastore::financial_data.get_index_stock_price(index);
        
        double balance = account.get_balance();

        if (price == 0.0)
        {
            std::ostringstream ss;
            ss << ",method=BrokerAccount::buy"
                << ",diagnostic_level=0"
                << ",index=" << index
                << ",price=" << price
                << ",message=Error_loading_data";
            Utilities::logline_threadsafe << ss.str();

            throw std::out_of_range("BrokerAccount::buy - Devide by zero");
        }

        if (price <= balance)
        {
            size_t saheres_to_purchase = balance / price;
            double cost_to_purchase = saheres_to_purchase * price;
            account.withdraw(cost_to_purchase);
            shares += saheres_to_purchase;
        }
    }

    void BrokerAccount::sell(size_t index)
    {
        double price = 0;

        if (financial_instrument_type == datastore::FinancialData::FinancialInstrumentType::Primary)
            price = datastore::financial_data.get_primary_stock_price(index);
        else
            price = datastore::financial_data.get_index_stock_price(index);

        if (shares > 0)
        {
            account.deposit(price * shares);
            shares = 0;
        }
    }

    void BrokerAccount::trace_execute(size_t index, unsigned long order_bitmask)
    {
        int buy_flag = order_bitmask & 0x01;
        int hold_flag = order_bitmask & 0x02;

        if ((buy_flag != 0) && (hold_flag == 0))
        {
            buy(index);
            {
                double price = 0;

                if (financial_instrument_type == datastore::FinancialData::FinancialInstrumentType::Primary)
                    price = datastore::financial_data.get_primary_stock_price(index);
                else
                    price = datastore::financial_data.get_index_stock_price(index);

                double balance = account.get_balance();
                double unrealized_gain = 0;

                if (shares > 0)
                    unrealized_gain += price * shares - seed_money;

                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=BrokerAccount.execute"
                        << ",index=" << index
                        << ",order_bitmask=" << order_bitmask
                        << ",order=buy"
                        << ",price=" << price
                        << ",shares=" << shares
                        << ",balance=" << balance
                        << ",unrealized_gain=" << unrealized_gain
                        << ",message=check";
                    Utilities::logline_threadsafe << ss.str();
                }
            }
        }

        else if ((buy_flag == 0) && (hold_flag == 0))
        {
            sell(index);
            {
                double price = 0;

                if (financial_instrument_type == datastore::FinancialData::FinancialInstrumentType::Primary)
                    price = datastore::financial_data.get_primary_stock_price(index);
                else
                    price = datastore::financial_data.get_index_stock_price(index);

                double balance = account.get_balance();
                double unrealized_gain = 0;

                if (shares > 0)
                    unrealized_gain += price * shares - seed_money;

                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=BrokerAccount.execute"
                        << ",index=" << index
                        << ",order_bitmask=" << order_bitmask
                        << ",order=sell"
                        << ",price=" << price
                        << ",shares=" << shares
                        << ",balance=" << balance
                        << ",unrealized_gain=" << unrealized_gain
                        << ",message=check";
                    Utilities::logline_threadsafe << ss.str();
                } 
            }
        }
    }

    /**
    * @brief Executes the given order on the broker account.
    *
    * @param index The index of the stock to be bought or sold.
    * @param order The order to be executed. The order is a bitmask where:
    *              - The least significant bit represents a buy order.
    *              - The second least significant bit represents a hold order.
    *
    * If the buy bit is set and the hold bit is not set, a buy order is executed.
    * If the buy bit is not set and the hold bit is not set, a sell order is executed.
    */
    void BrokerAccount::execute(size_t index, unsigned long order)
    {
        int buy_flag = order & 0x01;
        int hold_flag = order & 0x02;

        if ((buy_flag != 0) && (hold_flag == 0))
            buy(index);

        else if ((buy_flag == 0) && (hold_flag == 0))
            sell(index);
    }

    double BrokerAccount::unrealized_gain(size_t index) const
    {
        double price = 0;

        if (financial_instrument_type == datastore::FinancialData::FinancialInstrumentType::Primary)
            price = datastore::financial_data.get_primary_stock_price(index);
        else
            price = datastore::financial_data.get_index_stock_price(index);

        double balance = account.get_balance();

        if (shares > 0)
            balance += price * shares;

        return balance - seed_money;
    }
}
