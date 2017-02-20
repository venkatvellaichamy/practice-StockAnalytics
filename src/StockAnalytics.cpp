#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>

using namespace std;

struct Record {
	string security;
	int quantity;
	bool isBuy;
	float price;
};

bool readFile(ifstream &file) {
	
	// Check if the file exists
	bool ret = file.good();

	// Otherwise print error
	if(!ret) { 
		cout << "File not exist!" << endl;
	}

	return ret;
}

Record parseLineToRecord(string line) {
	string tmp = "";
	char tmpChar = ' ';
	int i;
	
	// Declare all columns
	int columnsCount = 4;
	int currentColumn = 0;

	// Parse each record to getvalues
	Record record;
	string::size_type sz;
		
	// Read the record string char by char
	for(i = currentColumn = 0 ; i < line.length(); i++) {
		tmpChar = line[i];
			
		// if character is a comma, store the value in appropriate column
		if(tmpChar == ',' || i == line.length() - 1) {
			switch(currentColumn) {
				case 0:
					record.security = tmp;
					break;
				case 1:
					record.quantity = stoi(tmp, &sz);
					break;
				case 2:
					record.isBuy = (tmp.compare("BUY") == 0);
					break;
				case 3:
					record.price = stof(tmp, &sz);
					break;
			}

			currentColumn++;
			tmp = "";
		}
		else {
			tmp.push_back(line[i]);
		}
	}

	return record;
}

vector<Record> parseCSV(ifstream &file) {
	int lineNumber = 0;
	string line;

	vector<Record> records;
	Record record;


	// Read till end of the file
	 while(!file.eof()) {
		line = "";

		// Read each line
		std::getline(file, line);

		// Skip first line as it is a column header
		if(++lineNumber == 1 || line.length() == 0) continue;
		record = parseLineToRecord(line);
		records.push_back(record);
	}

	// return all records
	return records;
}

map<string, float> doAnalytics(vector<Record> records) {
	// Have a hash map to store details of individual security
	map<string, float> securityHash;
 	Record record;

	// iterate through records
	for(int i = 0; i < records.size(); i++) {
		record = records[i];
		
		// if isBuy is true, multiply quantity and price and add it to the value in hash for that security
		if(record.isBuy == true) {
			securityHash[record.security] += record.quantity * record.price;
		} 
		// if isBuy is false, multiply quantity and price and subtract it to the value in hash for that security
		else {
			securityHash[record.security] -= record.quantity * record.price;
		}
	}

	return securityHash;
}

void displayRecords(map<string, float> hash) {
	for(map<string, float>::iterator i = hash.begin(); i != hash.end(); i++) {
		cout << i-> first << " : " << i->second << endl;
	}
}

int main() {
	ifstream file("assets/StockTransactions.csv");
	
	// Read the file
	int ret = readFile(file);
	
	if(ret) {
	 	// Parse the csv file to get each records' data
		vector<Record> records = parseCSV(file);
		
		// Do the computaion
		map<string, float> hash = doAnalytics(records);

		displayRecords(hash);
	}	

	return ret;
}
