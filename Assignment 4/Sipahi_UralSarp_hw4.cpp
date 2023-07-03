#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <queue>
#include <fstream>
#include "Graph.h"

using namespace std;


int main()
{
	string fname="words.txt";
	string word1,word2;
	cout<<"Enter first word: ";
	cin>>word1;
	cout<<"Enter second word: ";
	cin>>word2;

	while(word1[0]!='*'){
		Graph* graph;
		graph = new Graph();
		graph->graphbuild(fname);
		if (!graph->exist(word1, word2)){
                cout << "At least one of the words you've entered is not in the dictionary try other words!" << endl;
        }
		else{
			graph->build();
			graph->BFS(word1,word2);
			
		}
		cout<<"Enter first word: ";
		cin>>word1;
		if (word1[0] == '*') break;
		cout<<"Enter second word: ";
		cin>>word2;
	}


    return 0;
}