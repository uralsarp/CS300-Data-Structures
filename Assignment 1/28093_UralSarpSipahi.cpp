#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
using namespace std;



int find(vector<string> P, vector<queue<string>> Q, int queueidx){ //HANGÝ (queueidx) Queuenun önündekini bulmak istiyorsun?
	auto it=find(P.begin(),P.end(),Q[queueidx].front());
	return (it-P.begin());
}


int main(){
	string foldername;
	cout<<"Please enter the process folder name: ";
	cin>>foldername;
	cout<<"When all processes are completed, you can find execution sequence in "<<endl;
	cout<<"\""<<foldername<<"/output.txt\"."<<endl;

	string fname=foldername+"/"+"configuration.txt";
	string line;
	ifstream input;
	int quenum,pronum,ssize;

	input.open(fname.c_str());
	getline(input,line);
	istringstream strstream(line);
	strstream>>quenum;
	getline(input,line);
	istringstream strstream2(line);
	strstream2>>pronum;
	getline(input,line);
	istringstream strstream3(line);
	strstream3>>ssize;
	
	vector<string> P; //vector to keep the values of the files

	for (int i=1; i<=pronum; i++){
		string num=to_string(i);
		string filename=foldername+"/p"+num+".txt";
		string str,laststr;
		ifstream in;
		in.open(filename);
		getline(in,str);
		while (in){
			laststr+=str;
			getline(in,str);	
		}
		P.push_back(laststr);
	}

	vector<int> I; //vector to keep a track of the index we need to check for each process

	for (int l=0; l<pronum; l++){
		I.push_back(0);
	}
	
	vector<queue<string>> Q; //vector of queues

	for (int j=0; j<quenum; j++){
		Q.push_back(queue<string>());
	}
	
	
	for (int k=0; k<pronum; k++){
		Q[0].push(P[k]);
	}
	
	string outname=foldername+"/output.txt";
	ofstream outfile;
	outfile.open(outname);

	int processcount=0; //keep track of how many processes are terminated

	while (true){
		int i=0;
		int j=0;
		while(i<ssize){
			if(Q[j].empty()){
				j++;
				if(j==Q.size()){
					break;
				}
			}
			else{
				while (!Q[j].empty() && i<ssize){
					int idx=find(P,Q,j);
					int index=I[idx];
					if(P[idx][index]=='1' && P[idx][index+1]!='-'){
						string temp=Q[j].front();
						Q[j].pop();
						Q[j+1].push(temp);
						string num=to_string(idx+1);
						string Qquenum=to_string(Q.size()-(j+1));
						outfile<<"1, PC"<<num<<", Q"<<Qquenum<<endl;
						i++;
						I[idx]=I[idx]+1;
					}
					else if(P[idx][index]=='0' && P[idx][index+1]!='-'){
						string temp=Q[j].front();
						Q[j].pop();
						Q[j].push(temp);
						string num=to_string(idx+1);
						string Qquenum=to_string(Q.size()-j);
						outfile<<"0, PC"<<num<<", Q"<<Qquenum<<endl;
						i++;
						I[idx]=I[idx]+1;
					}
					else{
						Q[j].pop();
						string num=to_string(idx+1);
						string quenum=to_string(Q.size());
						if(processcount!=pronum-1){
							outfile<<"E, PC"<<num<<", QX"<<endl;
							processcount++;
						}
						else{
							outfile<<"E, PC"<<num<<", QX";
						}
						i++;
					}
				}
				j++;
			}
		}
		//when s is reached this part will be executed.
		int k=1;
		while(k<Q.size()){
			if(Q[k].empty()){
				k++;
			}
			else{
				while(!Q[k].empty()){
					int idx=find(P,Q,k);
					string temp=Q[k].front();
					Q[k].pop();
					Q[0].push(temp);
					string num=to_string(idx+1);
					string Qquenum=to_string(Q.size());
					outfile<<"B, PC"<<num<<", Q"<<Qquenum<<endl;
				}
				k++;
			}	
		}
		int h=0;
		bool empty=false;
		while(Q[h].empty()){
			h++;
			if(h==Q.size()){
				empty=true;
				break;
			}
		}
		if(empty==true)
			break;
	}
	outfile.close();
	return 0;
}