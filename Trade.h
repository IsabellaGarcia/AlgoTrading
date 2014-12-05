#ifndef TRADE_H
#define TRADE_H
#include "headfiles.h"
//Algorithms of Bollinger Band
class Trade{
	public:
		Trade(vector<int>, int, int,string);
		inline int action(float);					// Decision on buying/selling/keeping
		void calc_Bands();						    // Calculation of three bands
		//void computeBolling(int, int, int, float); 
		void processingTrade();						// Process of one-day trade
		char month(int);							// Determination of character for months
		float slippage_model(string, int);			// Order slippage model
		void processingTrade_1();					// For order slippage model 2
		void slippage_model_1(int);					// Order slippage model 2
		void next_price(ifstream& array_file);
		int trade_no();						// Return of total trade number;
		float PnL();							// Return PnL of today
		int s_trade();
		int trade_days_1();
		~Trade(){};
	private:
		void calc_SMA();							// Calculation of SMA of DURATION days
		void calc_SD();								// Calculation of SD of DURATION days
		float middle_Band;						    // Three bands 
		float upper_Band;
		float lower_Band;
		float SMA;				
		float SD;
		vector<int> past_Price;						// Close Prices of past DURATION days
		int index;						            // Index for dates;
		int duration;								// DURATION read from main function
		int stock;									// Number of stock in hand
		float asset;								// Remaining Money
		string file_name;
		float close;
		int total_trade;
		int sell_trade;
		int trade_days;
		float best_sell;
		float best_buy;
};
#endif