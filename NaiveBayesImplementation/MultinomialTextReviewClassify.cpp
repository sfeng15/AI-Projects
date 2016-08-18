#include <iostream>
#include <stack>   
#include <queue>       
#include <fstream>
#include <string>
#include <cmath> 
#include <algorithm>   
#include <vector>    
#include <limits> 
#include <map> 
#include <sstream>
#include <cstring>
#include <iomanip> 
#include <math.h>

using namespace std;

int smooth=30;
double uniqueGood=0.0;
double uniqueBad=0.0;

map<string, double> goodEmail;
map<string, double> badEmail; 

map<string, double> testhigh; 
double good=0.0;
double bad=0.0;
double goodnum = 0.0;
double badnum = 0.0;
double confusionMatrix[2][2];

vector<int> estimate;
vector<int> real;

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}

void parseTraining(){
	 for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            confusionMatrix[i][j] = 0.0;
        }
    }


	string line;

	ifstream myEmailFile("rt-train.txt");

	while ( getline(myEmailFile,line) ){
		vector<string> myVector = split(line, ' ');
		if (myVector[0] == "1")
		{
			goodnum = goodnum + 1.0;
			for (int i = 1; i < myVector.size(); i++)
			{
				vector<string> temp = split(myVector[i], ':');
				string name = temp[0];
				int value = atoi(temp[1].c_str());
				good=good+ value;

				map<string, double>::iterator it;
				it = goodEmail.find(name);
				if (it != goodEmail.end())  // find the name
				{
					goodEmail[name] = goodEmail[name] + value;
				}
				else{
					goodEmail[name] = value;
					uniqueGood=uniqueGood+1;
				}
			}
		}
		else{
			badnum = badnum + 1.0;
			for (int i = 1; i < myVector.size(); i++)
			{
				vector<string> temp = split(myVector[i], ':');
				string name = temp[0];
				int value = atoi(temp[1].c_str());
				bad=bad+value;

				map<string, double>::iterator it;
				it = badEmail.find(name);
				if (it != badEmail.end())  // find the name
				{
					badEmail[name] = badEmail[name] + value;
				}
				else{
					badEmail[name] = value;
					uniqueBad=uniqueBad+1;
				}
			}

		}
	}



	// naive bayes formula

	for (std::map<string,double>::iterator it = goodEmail.begin(); it != goodEmail.end(); it++){
		goodEmail[it->first] = (it->second+smooth)/ (good + smooth*uniqueGood);
	}
	for (std::map<string,double>::iterator it = badEmail.begin(); it != badEmail.end(); it++){
		badEmail[it->first] = (it->second + smooth) / (bad + smooth* uniqueBad);
	}

}







void parseTest(){

	string line;

	ifstream myEmailFile("rt-test.txt");


  
 
	while ( getline(myEmailFile,line) ){
		vector<string> myVector = split(line, ' ');
			double oneGood=0.0;
			double oneBad=0.0;
			for (int i = 1; i < myVector.size(); i++)
			{
				vector<string> temp = split(myVector[i], ':');
				string name = temp[0];
				int value = atoi(temp[1].c_str());
				
				map<string, double>::iterator it0;
				it0 = testhigh.find(name);
				if (it0 != testhigh.end())  // find the name
				{
					testhigh[name]= testhigh[name]+ 1;
				}
				else{
					testhigh[name]= 1;
				}

		



				map<string, double>::iterator it;
				it = goodEmail.find(name);
				if (it != goodEmail.end())  // find the name
				{
					oneGood = oneGood + goodEmail[name] * value;
				}
				map<string, double>::iterator it2;
				it2 = badEmail.find(name);
				if (it2 != badEmail.end())  // find the name
				{
					oneBad = oneBad + badEmail[name] * value;
				}
				//oneGood=oneGood/(goodnum/(goodnum+badnum));
				//oneBad=oneBad/(badnum/(goodnum+badnum));
				
			}

			real.push_back(  atoi(myVector[0].c_str() )  );
			if(oneGood>oneBad){
				estimate.push_back(1);
			}
			else{
				estimate.push_back(-1);
			}

			int realback=real.back();
			int estimateback=estimate.back();
			if(realback==1 &&  estimateback==1){
				confusionMatrix[0][0] = confusionMatrix[0][0] +1;
			}
			else if(realback==1 &&  estimateback==-1){
				confusionMatrix[0][1] = confusionMatrix[0][1] +1;
			}
			else if(realback==-1 &&  estimateback==1){
				confusionMatrix[1][0] = confusionMatrix[1][0] +1;
			}
			else if(realback==-1 &&  estimateback==-1){
				confusionMatrix[1][1] = confusionMatrix[1][1] +1;
			}


		}


	// ofstream myfile0;
 //  	myfile0.open ("toSort.txt");
 //  	for (std::map<string,double>::iterator it = testhigh.begin(); it != testhigh.end(); it++){
	// 	myfile0 <<it->first<<" "<<it->second<<endl;
	// }
	// myfile0.close();


}



void print(){
	ofstream myfile0;
  	myfile0.open ("toSortGoodEmail.txt");
  	for (std::map<string,double>::iterator it = goodEmail.begin(); it != goodEmail.end(); it++){
		myfile0 <<it->first<<" "<<it->second<<endl;
	}
	myfile0.close();

	ofstream myfile1;
  	myfile1.open ("toSortBadEmail.txt");
  	for (std::map<string,double>::iterator it1 = badEmail.begin(); it1 != badEmail.end(); it1++){
		myfile1 <<it1->first<<" "<<it1->second<<endl;
	}
	myfile1.close();
}



int main()
{
	
	parseTraining();
	parseTest();
	double wrong=0.0;
	for(int i=0;i<real.size();i++){
		if(real[i]!=estimate[i]){
			wrong=wrong+1;
			cout<<i<< ": "<< real[i]<<", "<<estimate[i]<<endl;
		}
	}

	double per=(real.size()-wrong)/real.size();
	cout<<per<<endl;

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			cout << confusionMatrix[i][j] << " ";
		}
		cout << endl;
	}

	print();
	// cout<<"estimate: ";
	// for(int i=0;i<10;i++){
	// 	cout << estimate[253+i] << " ";
	// }
	// cout << endl;
	return 0;
}
