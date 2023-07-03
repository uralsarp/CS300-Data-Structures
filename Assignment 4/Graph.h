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
#include <stack>

using namespace std;

struct WordNode
{
   
    string word;
   
    vector<int> neighbour;
    
    bool visited;

	WordNode* parent;

};

class Graph{
	public:
		vector<string> dictionary;
		vector<WordNode*> graph;

		void graphbuild(string fname){

			fstream input;
			input.open(fname);
			string temp;
			while (input >> temp){
				dictionary.push_back(temp);
			}
	
			for (int i=0; i<dictionary.size();i++){ //fill the graph with nodes
				WordNode* Node = NULL;
				Node = new WordNode();
				Node->word = dictionary[i];
				Node->visited = false;
				Node->parent=NULL;
				graph.push_back(Node);
			}
			
		}

		bool exist(string w1, string w2){
			bool word1 = false;
			bool word2 = false;
			for (int i = 0; i < dictionary.size(); i++){
				if (dictionary[i] == w1)
					word1 = true;
				if (dictionary[i] == w2)
					word2 = true;
			}
			if (word1 == true && word2 == true)
				return true;
			else
				return false;
		}


		int min3(int x, int y, int z) { 
				return min(min(x, y), z); 
		}
		// Distance between 2 strings
		int editDist(string str1, string str2, int m, int n)  //graph will have edges between nodes that are 1 edit distance away (levenstein distance function)
		{	
			if (m == 0)
				return n;
  	
			if (n == 0)
				return m;
 	
			if (str1[m - 1] == str2[n - 1])
				return editDist(str1, str2, m - 1, n - 1);

			return 1
				  + min3(editDist(str1, str2, m, n - 1), // Insert
					editDist(str1, str2, m - 1, n), // Remove
						 editDist(str1, str2, m - 1,
						  n - 1) // Replace
					 );
		}
		
		void build() //add edges between nodes
		{
			for (int i = 0; i < graph.size(); ++i)
				for (int j = i + 1; j < graph.size(); ++j)
						if (editDist(graph[i]->word, graph[j]->word,graph[i]->word.size(),graph[j]->word.size()) < 2) {
						graph[i]->neighbour.push_back(j);
						graph[j]->neighbour.push_back(i);
					}
			
		}

		void BFS(string w1, string w2) { //breadth first search
			
			bool result = false;

			int i=0;
			while (graph[i]->word != w1){ //find word1(starting word) in the graph
				i++;
			}
			graph[i]->visited = true; 
			queue<int> check;
			check.push(i);

			stack<string> words;

			while (!check.empty()) {
				
				int currentNode = check.front(); 
				check.pop();
				
				if (graph[currentNode]->word == w2) { //found the final word (destination)
					result = true;
					while (graph[currentNode]->parent!=NULL){  //iterate over parents
						words.push(graph[currentNode]->word);
						graph[currentNode]=graph[currentNode]->parent;
					}
					words.push(graph[currentNode]->word); //push the first word
					break;

				}

				// Go through all neighbours of the current node
				for (const int next : graph[currentNode]->neighbour){
					// If the neighbour node has not yet been visited
					if (!graph[next]->visited) {
						graph[next]->visited = true;

						graph[next]->parent=graph[currentNode]; //update the nodes' parent

						check.push(next);
					}
				}
			}
			if(result){ //if there is a path
				printgraph(words);
			}
			else{
				cout<<"You cannot reach from "<<w1<<" to "<<w2<<endl;
			}
		}
		
		void printgraph(stack<string> words){

			string word1,word2;
			int counter=0;
			while(words.size()!=1){
				if (counter == 0){
					cout << words.top() << endl;
					counter++;
					continue;
				}	
				word1=words.top();
				words.pop();
				word2=words.top();

				if(word1.length()==word2.length()){ //its a change
					for(int i=0; i<word1.length();i++){
						if (word1[i]!=word2[i]){
							cout<<word2<<" (change "<<word1[i]<<" at position "<<i+1<<" to "<<word2[i]<<")"<<endl;
							break;
						}
					}
				}
				else if(word1.length()+1==word2.length()){ //its an addition	
					for(int i=0; i<word2.length();i++){
						if (word1[i]!=word2[i]){
							cout<<word2<<" (insert "<<word2[i]<<" after position "<<i<<")"<<endl;
							break;
						}
					}
				}
				else{ //its a deletion
					for(int i=0; i<word1.length();i++){
						if (word1[i]!=word2[i]){
							cout<<word2<<" (delete "<<word1[i]<<" at position "<<i+1<<")"<<endl;
							break;
						}
				
					}
		
				}
			
			}	
		}
};