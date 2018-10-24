#ifndef _BROKER__H__
#define _BROKER__H__

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <cstdarg>

#include "..\Database\SqlConnection.h"
#include "Account.h"
#include "StockPrice.h"

namespace finance
{
	class Broker
	{
	public:
		enum OrderType
		{
			Market = 0,
			Limit
		};

		static const double MarketTransactionFee;
		static const double LimitTransactionFee;

		Broker();
		Broker(double openingBalance/*, std::string archiveFilename*/);
		~Broker();

		double Cash();
		_ULonglong Quantity();
		double Fee(OrderType orderType);

		static StockPrice GetStockPrice(unsigned int index);
		void Buy(StockPrice& stockPrice);
		void Sell(StockPrice& stockPrice);
		double Unrealized(StockPrice& stockPrice);
		double RealizedROI();
//		double CalculateProfit(StockPrice* stock1, StockPrice* stock2, double commitment, double minimumDailyProfit, OrderType orderType, _ULonglong& quantity);
		void Reset(double openingBalance);
		static double GetStockData( unsigned int row, int column );
		static int GetNumberOfTrainingDataRows();
		std::string GetColumnName( int column );

		unsigned int GetIndicator( unsigned int _indicatorId, unsigned int _dataRowIndex );



//		int GetNumberOfTestDataRows();



	private:
		friend class boost::serialization::access;

		struct _ColumnInfo
		{
			int		index;
			double	min;
			double	max;
			double	m;
			double	b;

			template<class Archive>
			void serialize(Archive & ar, const unsigned int version)
			{
				ar & index;
				ar & min;
				ar & max;
				ar & m;
				ar & b;
			}
		};
		typedef struct _ColumnInfo ColumnInfoType;

		typedef std::map< std::string, _ColumnInfo > ColumnInfoMapType;

		//static const std::string archiveFilename_;
		static std::vector<StockPrice> trainingStockPrices_;
		static ColumnInfoMapType columnInfoMap_;

		void buy(double price, double fee);
		void buy(double price, _ULonglong quantity, double fee);

		void loadStockPrices();
		void loadTechnicalData( const std::string& _tableName, const std::string& _sqlStatement, std::vector<std::string> _keyField );

		bool loadStockDataFromArchive();
		bool saveStockDataToArchive();

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & columnInfoMap_;
			ar & trainingStockPrices_;
		}

		Account *account_;

		double openingBalance_;


		//static const std::string archiveTestFilename_;
		//static std::vector<StockPrice> testStockPrices_;

	};
}

#endif