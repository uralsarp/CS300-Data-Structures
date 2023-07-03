#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

int main()
{

	ifstream input;
	input.open("compout");
	ofstream output;
	output.open("decompout");

	vector<string> arr(4096);
	string temp="";
	for (int i = 0;	i< 256; i++){//fill the table with ascii codes
		temp = char(i);
		arr[i]=temp;
	}

	int start=256;
	string currtxt;
	string prevtxt;
	int num;

	input>>num; //first character will always be in the dictionary no need for a check
	output<<arr[num];
	prevtxt=arr[num];

	
	while(input>>num){

		if (arr[num] != ""){ //if the value is in the dictionary  ||  decompress first then add new value prevtext+first_char(currenttxt)
			output<<arr[num];
			currtxt=arr[num].substr(0,1);
			arr[start]=prevtxt+currtxt;
			prevtxt=arr[num];
			start++;
		}
		else{ //add new value prevtext+first_char(prevtext) then decompress
			arr[start]=prevtxt+prevtxt.substr(0,1);
			output<<arr[start];
			prevtxt=arr[num];
			start++;
		}
	}

	input.close();
	output.close();

	return 0;
}