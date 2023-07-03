#include <iostream>
#include <string>
#include <fstream>
#include "HashTable.h"

using namespace std;

int main(){

	string error="Item not found"; //sentinel value
	HashTable<string> table(error, 4096); //initialize the table

	ifstream input;
	input.open("compin");
	ofstream output;
	output.open("compout");

	
	string temp="";
	for (int i = 0;	i< 256; i++){//fill the table with ascii codes
		temp = char(i);
		table.insert(temp, i);
	}
	int start=256; //the program will start to put values in the dictionary starting with the num 256
	string p="";
	string c="";
	string pc="";
	char ch;
	while(!input.eof()){
		input.get(ch); 
		c=ch;
		pc=p+c;

		if (table.find(pc)!=-1){ // if its in the dictionary
			p=pc;
		}
		else{
			table.insert(pc,start);	//first parameter is the value second is the place that its going to be placed in the table
			start++;
			output<<table.find(p)<<" "; //dont print out pc yet since it has been just added to the dictionary
			p=c;
		}
	}
	if (p==pc){	//if the last code was in the dictionary
		p=p.substr(0,p.size()-1);
		output<<table.find(p); //print out the last code
	}
	output.close();
	input.close();
	return 0;
}