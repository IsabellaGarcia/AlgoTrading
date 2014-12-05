#include "headfiles.h"
#include "Trade.h"
#define DURATION 20
#define FILE_NO 80

//setup file name and its index
void setup_Name(vector<string>&);
void calc_result(int, long int, double, int, int);
//-------------------- ONLY FOR FUNCTION 1 ---------------------
//Read all the files to find all the close prices
void read_close_Price(vector<int> &,vector<string>);

//-------------------- ONLY FOR FUNCTION 2 ---------------------
void setup_close_Price(vector<int> &);

int main(){
	vector<string> list_Name;
	vector<int> close_Price;
	// variables below is for the final result
	long int total_trade = 0;
	double PnL = 0.0;
	int winning_day = 0;
	int total_days = 0;
	setup_Name(list_Name);
	int choose = 2;
	cout << "Function 1: Get all the close price from *.csv" << endl;
	cout << "Function 2: Conduct transactions (default)" << endl;
	//cin >> choose
	if(choose == 1)
		read_close_Price(close_Price,list_Name);
	else{
		setup_close_Price(close_Price);
		for(int i = 21; i < 80; i++){
		Trade t1(close_Price, i, DURATION, list_Name.at(i));
		t1.calc_Bands();
		t1.processingTrade_1();	
		if(t1.PnL() > 0)
		{
			winning_day++;
		}
		if(t1.PnL()!= 0){
			total_days++;
		}
		total_trade = t1.trade_no() + total_trade;
		PnL = (double)t1.PnL() + PnL;
		}
	}

	calc_result(winning_day, total_trade, PnL, 60, total_days);
	while(1);
	return 0;
}

void setup_Name(vector<string>& list_Name){
	long long index = 20131104;
	int weekly = 0;
	int i = 1;

	//November index
	while(index < 20131131 ){
		if(weekly < 5){
		list_Name.push_back(to_string(index));
		weekly++;
		index++;
		i++;
		}
		else{
		weekly = 0;
		index += 2;
		}
	}

	// December index
	index = 20131202;
	weekly = 0;
	while(index < 20131232 ){
		if((weekly < 5) && ((index!=20131225)&&(index!=20131226))){
			list_Name.push_back(to_string(index));
			weekly++;
			index++;
			i++;
		}
		else if(((index==20131225)||(index==20131226))){
			weekly++;
			index++;
		}
		else{
			weekly = 0;
			index += 2;
		}
	}
	// January index
	weekly = 3;
	index = 20140102;
	while(index < 20140131 ){
		if((weekly < 5)){
			list_Name.push_back(to_string(index));
			weekly++;
			index++;
			i++;
		}
		else{
		weekly = 0;
		index += 2;
		}
	}

	//Febrary index
	weekly = 1;
	index = 20140204;
	while(index < 20140229 ){
		if((weekly < 5)){
			list_Name.push_back(to_string(index));
			weekly++;
			index++;
			i++;
		}
		else{
		weekly = 0;
		index += 2;
		}
	}
}

void read_close_Price(vector<int>& close_Price, vector<string> list_Name)
{
	ifstream infile;

	//write all close prices into closePrice.txt file
	ofstream outfile;
	outfile.open("closePrice_1.txt");

	string c = ".csv";
	float temp_save = 0;
	istringstream iss;
	float last_price;
	int time;
	int print = 0;
	// read csv files one by one
	for(int i = 61; i < FILE_NO; i++){
		string line;
		//reset time and temporary value;
		time = 0;
		temp_save = 0;
		last_price = 0;
		 //create file name
		string temp = "data\\";
		temp = temp + list_Name.at(i);
		temp = temp + c;
		cout << list_Name.at(i) + c << endl;

		//open this file
		infile.open(temp);
		if ( !infile){
		cout << "Could not open file" << endl;
		}
		//successfully open
		else
		{
			//skip the first 60000 lines
			for(int z = 0; z < 60000; z++)
				getline(infile, line);
				
		    //interpret line by line from 60000 to end line
			while(!infile.eof()){
				getline(infile, line);
				//cout << line << endl;
				iss.str(line);
				iss >> time;
				iss.ignore(7);
				//find the lose price of today(last one on 161500)
				if(time < 161500){
					iss >> temp_save;
					 //cout << time   << " "<< temp_save << endl;
					continue;
				}
				else if(time == 161500){
					iss >> last_price;
				}
				else if(time > 161500){
					break;
				}
			}


		if((time > 161500)&&(last_price == 0)){
			last_price = temp_save;
		}
		if (time < 161500){
		//cout << time   << " "<< temp_save;
			last_price = temp_save;
		}
		close_Price.push_back(last_price);
		cout << (int)last_price << endl;
		if(print < 10){
			outfile <<  (int)last_price ;
			outfile << " ";
			print ++;
		}
		else
		{	outfile << "\n";
			outfile <<  (int)last_price ;
			outfile << " ";
			print = 0;
		}
		//while(1);
		//close this file
		infile.close();
		}
	}
	outfile.close();
}

void setup_close_Price(vector<int> &close_Price){
	//open txt file which includes all closePrice
	ifstream infile;
	int print = 0;
	infile.open("closePrice.txt");
	int price;

	for(int i = 0; i < 8; i++){
		string line;
		getline(infile,line);
		istringstream iss;
		iss.str(line);
		print = 0;
		for(; print < 10; print++){
			iss >> price;
			close_Price.push_back(price);
	    }		
	}
	
}

void calc_result(int winning_day, long int total_trade, double PnL, int duration, int total_days){
	cout << "Total Trades: " << total_trade << endl;
	cout << "Total Points Earned: " << PnL << endl;
	cout << "Total Trading Day: " << duration << endl;
	cout << "Avg. Points Earned per Day: " << PnL/(double)duration << endl;
	cout << "Avg. Points Earned per Tranctions: " << PnL/(double)total_trade << endl;
	cout << "Winning per Traded Days:" << winning_day/(float)total_days << endl;
	//cout << "Winning Percentage per transaction" << PnL/(double)total_trade << endl;
	cout << "Winning: " << (float)winning_day/(float)duration << endl;
}

