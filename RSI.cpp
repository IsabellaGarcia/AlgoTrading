#include "RSI.h"
#include "headfiles.h"

RSI::RSI(vector<int> prices, int d, string file, int index_1)
	      : duration(d), index(index_1){
	stock = 0;
	asset = 0.0;
	average = 0.0;
	total_trade = 0;
	int start = index_1 - duration;
	construct_filename(file);
	vector<int>::iterator it = prices.begin();
	it = it + start;
	for(int i = 0; i < duration; i++){
		past_Price.push_back(*it);
		it++;
	}
	close = prices.at(index);	
	calc_RS();
	calc_RSI();
}

inline string RSI::construct_filename(string file){
	string temp = "data\\";
	string c = ".csv";
	cout << "date: " << file << endl;
	file_name = temp + file + c;
	return file_name;
}

inline bool RSI::compare(float a, float b){
	return (a>b ? true : false);
}

void RSI::calc_RS(){
	float gain = 0.0;
	float loss = 0.0;
	float sum = 0.0;
	vector<int>::iterator it = past_Price.begin();
	int x;
	x = *it;
	sum += x;
	cout << "close price today = " << x << endl;
	it++;
	for(;it!= past_Price.end(); it++){
		if(compare(x, (*it)))
			loss = loss + x - (*it);
		else
			gain = gain + (*it) - x;
		x = (*it);
		sum += x;
	}
	gain = gain/(float)duration;
	loss = loss/(float)duration;
	RS = gain/loss;
	average = sum/duration;
}

inline void RSI::calc_RSI(){
	RSI_1 = 100 - 100/(1 + RS);
}

void RSI::Transaction(){
	cout << "Processing.."<< endl;
	ifstream infile;
	infile.open(file_name);
	if (!infile){
		cout << "Could not open file" << endl;
	}	
	string line;
	getline(infile, line);
	string current_line = line;					// for price

	while(!infile.eof()){	
		string price_line;							// current line 
		getline(infile, price_line);
		if(condition(current_line)){ //condition == true
			//get the buy price and sell price
			int act = action(current);
			if((act == 1)&&(stock == 1)){
				slippage(price_line,1);
				stock--;
				asset = asset + sell_price;
				total_trade++;

			}
			else if((act == -1)&&(stock==0)){
				slippage(price_line,-1);
				stock++;
				asset = asset - buy_price;
				total_trade++;
			}
			else if(act == 0){
			}
			current_line = price_line;
		} // condition == false
		else{
			current_line = price_line;
		}
	}
	if(stock>0){
		asset = asset + stock * close;
		stock = 0;
		total_trade++;
	}
}

void RSI::slippage_model(){
}

inline int RSI::action(float current){
	if((RSI_1>70) && (current > 1.1*average))
	return 1;
	if((RSI_1>70) && (current < 0.9*average))
	return -1;
	if((RSI_1<30)&&(current < 0.9*average))
	return -1;
	if((RSI_1<30)&&(current > 1.1*average))
	return 1;
	
	return 0;
}

void RSI::slippage(string next_line, int act){
		string x;
	// sell
	if(act == 1){
		x = "B";
	}
	// buy
	else if(act == -1){
		x = "A";
	}
	// find the location of A or B
	size_t found = next_line.find(x);
	if (found!=std::string::npos){}
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
	if(act == 1){
		for(int i = 0; i < 4; i++){
			iss.ignore(1);
			iss >> qty;
			iss.ignore(1);
			iss >> value_1;
			transaction_price = max(transaction_price, value_1);
		}
	sell_price = transaction_price;
	}
	else if(act == -1){
		for(int i = 0; i < 4; i++){
			iss.ignore(1);
			iss >> qty;
			iss.ignore(1);
			iss >> value_1;
			transaction_price = min(transaction_price, value_1);	
		}
	buy_price = transaction_price;
	}
}

bool  RSI::condition(string line){
	istringstream iss;
	iss.str(line);
	int time;
	iss >> time;
	if((time < 91500)||(time >161500)){
		return false;
	}
	else{
		string a;
		iss >> a;
		char month_index = month(index);
		if(a.at(4) == month_index){
			istringstream iss_1;
			iss_1.str(a);
			iss_1.ignore(7);
			float current_price;
			iss_1 >> current_price;
			if(current_price >= 99999){
				return false;
			}
			else{
				current = current_price;
				return true;
			}
		}
		else{
			return false;
		}
	}







return 1;
}

char RSI::month(int index){

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

float  RSI::PnL(){return asset;}
