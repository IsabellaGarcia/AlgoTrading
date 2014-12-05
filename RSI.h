#ifndef RSI_H
#define RSI_H
#include "headfiles.h"
class RSI{
	public:
		RSI(vector<int>, int, string, int);    // Constructor
		inline string construct_filename(string);
		inline bool compare(float, float);	   // Compare a and b
		void calc_RS();
		inline void calc_RSI();
		void Transaction();
		void slippage_model();
		inline int action(float);
		void slippage(string,int);
		bool condition(string);
		char month(int);
		float PnL();
	private:
		vector<int> past_Price;
		float close_price;
		string file_name;
		int duration;
		int index;
		float close;
		float RS;
		float RSI_1;
		int stock;
		float asset;
		float buy_price;
		float sell_price;
		float current;
		float average;
		int total_trade;
};
#endif