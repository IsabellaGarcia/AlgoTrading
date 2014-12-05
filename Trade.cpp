#include "Trade.h"
#include "headfiles.h"

Trade::Trade(vector<int> close_Price, int index, int duration,string file) : index(index), duration(duration){
	int start = index - duration;
	stock = 0;
	asset = 0.0;
	// for RSI
	best_sell = 0.0;
	best_buy = 999999;
	total_trade = 0;
	trade_days = 0;
	sell_trade = 0;
	string temp = "data\\";
	string c = ".csv";
	cout << "date: " << file << endl;
	file_name = temp + file + c;
	vector<int>::iterator it = close_Price.begin();
	it = it + start;
	for(int i = 0; i < duration; i++){
		past_Price.push_back(*it);
		it++;
	}
	close = close_Price.at(index);
}

void Trade::calc_SMA(){
	float avg = 0.0;
	if(past_Price.size() == 0){
		SMA = avg;
	}
	//calculate the average value of DURATION close prices
	else{
		vector<int>::iterator it = past_Price.begin();
		for(; it != past_Price.end(); it++){
			avg += (*it);
		}
	}
	SMA = (float)avg/past_Price.size();
	//cout << "length = " << past_Price.size() << endl;
	//cout <<"SMA = " << SMA << endl;
}

void Trade::calc_SD(){
	calc_SMA();
	if(past_Price.size() == 0){
		SD = 0.0;
	}
	float dev = 0.0;
	vector<int>::iterator it = past_Price.begin();
	for(; it != past_Price.end(); it++){
		float a = pow(((*it) - SMA),(float)2.0);
			dev += a;			
	}
	//cout << dev << endl;
	dev = dev/(float)(past_Price.size());
	dev = pow(dev, (float)0.5);
	SD = dev;
}

void Trade::calc_Bands(){
		calc_SMA();
		calc_SD();
		middle_Band = SMA;
		upper_Band = SMA + (float)0.5 * SD;
		lower_Band = SMA - (float)1 * SD;
}

inline int Trade::action(float traded_price){
	/*
     take different actions
	 1  -> sell;
	 0  -> no action
	 -1 -> buy
	*/
	if(traded_price > upper_Band){
		return 1;
	}
	else if(traded_price < lower_Band){
		return -1;
	}
	else{
		return 0;
	}

}

void Trade::processingTrade(){
	ifstream infile;
	infile.open(file_name);
	//int trend = 0;
	if (!infile){
		cout << "Could not open file" << endl;
	}
	cout << "Processing.."<< endl;
		string line;				 // current line;
		string line_1;				 // next line for slippage use
		getline(infile, line);      
		while(!infile.end){
			getline(infile, line_1);
			istringstream iss;
			iss.str(line);

			int time;
			int hold_con = 0;
			iss >> time;
			if((time < 91500)||(time >161500)){
				line = line_1;
				//trend = 0;
				continue;
				hold_con = 0;
			}
			// correct time period
			else{
				string a;
				iss >> a;
				char month_index = month(index);
				//correct month period
				if(a.at(4) == month_index){
					istringstream iss_1;
					iss_1.str(a);
					iss_1.ignore(7);
					float current_price;
					iss_1 >> current_price;
					if(current_price >= 99999){
						line = line_1;
						hold_con = 0;
						continue;
					}
					int action_n = action(current_price);
					if(action_n == 0){
						//trend = 0;
						line = line_1;
						hold_con = 0;
						continue;
					}
					else{ //transaction begins
						if((action_n == 1)&&(stock > 0)){
							slippage_model(line_1, 1);
							total_trade++;
							line = line_1;
							hold_con = 0;
						}
						else if((action_n == 1)&&(stock == 0)){
							hold_con++;
							if(hold_con >= 4)
							{
								slippage_model(line_1, -1);
								total_trade++;
								hold_con = 0;
							}
							line = line_1;
						}
						else if((action_n == -1)&&(stock >= 0)&&(stock < 1)){
							slippage_model(line_1, -1);
							total_trade++;
							line = line_1;
							hold_con = 0;
						}
						else if((action_n == -1)&&(stock >= 1)){
							line = line_1;
							hold_con = 0;
						}
						/*if((action_n == 1)&&(stock == 0))
						{
							cout << "WANT TO SELL BUT NO STOCK IN HAND"<< endl;
							trend++;
							if(trend >= 10)
							{
								slippage_model(line_1, -1);
								trend = 0;
								total_trade++;
							}
							line = line_1;
						}
						else{
							slippage_model(line_1,action_n);	
							trend = 0;
							total_trade++;
							line = line_1;
						}*/
					}
				}
				else{ //month incorrect
					line = line_1;
					continue;
				}
			}//time period incorrect
		}// end while
		if(total_trade >0 )
			trade_days++;
		// at the end of the day, if still have stock in hand, sell them all at close_price;
		cout << "stock = " << stock << endl;
		cout << "asset = " << asset << endl;
		cout << "close price = " << close << endl;
		if(stock > 0){
			asset = asset + close * stock;
			total_trade++;
			sell_trade++;
			stock = 0;
		}
		cout << "At the end of this day, sell all stocks, PnL = " << asset << endl;
		infile.close();
}

void Trade::processingTrade_1(){
	//Read in each line for deciding transaction
	ifstream infile;
	infile.open(file_name);
	if (!infile){
		cout << "Could not open file" << endl;
	}
	//Read in each line for best price
	ifstream array_file;
	array_file.open(file_name);
	if (!array_file){
		cout << "Could not open array file" << endl;
	}
	
	//Calculate the first average price
	
		for(int i = 0; i < 10; i++){
			string array_line;
			getline(array_file, array_line);
			istringstream array_iss;
			array_iss.str(array_line);
			int time_temp;
			array_iss >> time_temp;
			array_iss.ignore(7);
			float new_price;
			array_iss >> new_price;
			if((new_price > best_sell)&&(new_price<=99999)){
				best_sell = new_price;
			}
			if(new_price < best_buy){
				best_buy = new_price;
			}
		}
	cout << "Processing.."<< endl;

	//Read in line
	while(1){
			string line;
			getline(infile, line);	
			if(infile.eof())
				break;
			else{
				istringstream iss;
				iss.str(line);
				int time;
				iss >> time;
				if((time < 91500)||(time >161500)){
					next_price(array_file);
					continue;
				}
				else{//right time
					string a;
					iss >> a;
					char month_index = month(index);
					if(a.at(4) == month_index){
						//now read in current price(last price)
						istringstream iss_1;
						iss_1.str(a);
						iss_1.ignore(7);
						float current_price;
						iss_1 >> current_price;	
						if(current_price >= 99999){
							next_price(array_file);
							continue;
						}
					else{//right current price
						int action_n = action(current_price);
						if(action_n == 0){
							next_price(array_file);				
						}
						else if((action_n == 1)&&(stock == 1)){//1  -> sell
							total_trade++;
							next_price(array_file);
							slippage_model_1(1);
							
							continue;
						}
						else if((action_n == 1)&&(stock == 0)){
							//cout << "no stock too sell " << endl;
							continue;
						}
						else if((action_n == -1)&&(stock == 0)){//1  -> buy
							//cout << "buy" << endl;
							total_trade++;				
							next_price(array_file);
							slippage_model_1(-1);
							
							continue;
						}
						else if((action_n == 0)&&(stock == 1)){
							//cout << "Stock full " << endl;
							continue;
						}
					}
				}
				else{//wrong month
						next_price(array_file);
					continue;
				}
			}
		}
	}
		cout << "stock = " << stock << endl;
		cout << "asset = " << asset << endl;
		cout << "close price = " << close << endl;
		if(stock > 0){
			asset = asset + close * stock;
			total_trade++;
			sell_trade++;
			stock = 0;
		}
		cout << "At the end of this day, sell all stocks, PnL = " << asset << endl;
		infile.close();
		array_file.close();
		
}

void  Trade::next_price(ifstream& array_file){
	// read 0 - 9 last prices for Processing line 0
	//array file and array line ------> for read in price;
	string array_line;
	getline(array_file, array_line);
	if(!array_file.eof()){
	//cout << array_line << endl;
	istringstream array_iss;
	array_iss.str(array_line);
	int time_temp;
	array_iss >> time_temp;
	string a;
	array_iss >> a;
	char month_index = month(index);
	if(a.at(4) == month_index){
		istringstream iss_1;
		iss_1.str(a);
			iss_1.ignore(7);
			float new_price;
			iss_1 >> new_price;
			if((new_price > best_sell)&&(new_price<=99999)){
				best_sell = new_price;
				//cout << "Best sell price change to " << best_sell << endl;
			}
			if(new_price < best_buy){
				best_buy = new_price;
			//	cout << "Best buy price change to " << best_buy << endl;
			}
		}
	}
	else{
		//cout << "end " << endl;
	}
}

char Trade::month(int index){

	char A;
	if((index >= 0)&&(index <= 18)){
		A = 'X';
	}
	else if((index >= 19)&&(index <= 38)){
		A = 'Z';
	}
	else if((index >= 39)&&(index <= 59)){
		A = 'F';
	}
	else if((index >= 60)&&(index <= 78)){
		A = 'G';
	}
	else if((index == 79)){
		A = 'H';
	}
	return A;
}

float Trade::slippage_model(string next_line, int trade_type){
	string x;
	// sell
	if(trade_type == 1){
		//cout << " sell at ";
		x = "B";
	}
	// buy
	else if(trade_type == -1){
		//cout << " buy at ";
		x = "A";
	}
	// find the location of A or B
	size_t found = next_line.find(x);
	if (found!=std::string::npos){}
  //  std::cout << "first 'A' found at: " << found << '\n';
	istringstream iss;
	iss.str(next_line);
	iss.ignore(found + 2);

	// read in first bid/ask price
	float value_1;
	float transaction_price;
	iss >> value_1;
	int qty;
	transaction_price = value_1;
	//Read in other 4 Bid/Ask prices, find the best one;
	// find the best sell price
	if(trade_type == 1){
		for(int i = 0; i < 4; i++){
			iss.ignore(1);
			iss >> qty;
			iss.ignore(1);
			iss >> value_1;
			transaction_price = max(transaction_price, value_1);
		}
		asset = asset + transaction_price * stock;
		stock = 0;
		sell_trade++;
	}
	// find the best buy price
	else if(trade_type == -1){
		for(int i = 0; i < 4; i++){
			iss.ignore(1);
			iss >> qty;
			iss.ignore(1);
			iss >> value_1;
			transaction_price = min(transaction_price, value_1);	
		}
		//cout << "Buy at " << transaction_price << "  asset =" << asset<< endl;	
		asset = asset - transaction_price * 1;
		stock++;
	}
	//cout << transaction_price << endl;

	return transaction_price;
}

void Trade::slippage_model_1(int action){
	if(action == 1){
		stock--;
		asset = asset + best_sell * 1;
		cout <<"Buy at "<< best_sell << ", now asset" << asset << endl;
	}
	//buy ---> find the lowest price
	else if(action == -1){
		stock++;
		asset = asset - best_buy * 1;
		cout <<"Buy at " << best_buy << ", now asset " << asset << endl;
	}
}

int Trade::trade_no(){
	return total_trade;
}

float Trade::PnL(){
	return asset;
}

int Trade::s_trade(){
	return sell_trade;
}

int Trade::trade_days_1(){
	return trade_days;
}