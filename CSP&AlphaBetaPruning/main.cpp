#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "table.h"
#include "tree.h"

using namespace std;



table parse_table()
{
	table map;

	string line;
  	ifstream myfile ("Sevastopol.txt");

  	int lineNum=0;
  	while ( getline (myfile,line) )
    {
        string buf; 
    	stringstream ss(line);

    	vector<int> tokens; 
    	while (ss >> buf){
    		tokens.push_back(stoi(buf));
    		
    	}


        for(int i=0;i<(int)tokens.size();i++){
        	map.board[lineNum][i].value=tokens[i];
        }	
        lineNum=lineNum+1;
    }
    myfile.close();

   	return map;

}


int main()
{
	table map=parse_table();
	int turn = 1; // blue first;
	Node * go=new Node(turn,map);
	go->addChildren();
	go->select();

	map=go->selectPtr->mytable;
	map.printTable();
	delete go;
	turn=turn*(-1);

	for(int i=0;i<35;i++){
		go=new Node(turn,map);
		go->addChildren();
		go->select();

		map=go->selectPtr->mytable;
		map.printTable();
		delete go;
		turn=turn*(-1);
	}

	/*for(int i=0;i<6;i++){
		for(int j=0;j<6;j++){
				cout<<map.board[i][j].value;
				cout << " ";
		}
		cout << endl;
	}*/

	cout << map.greenValue << endl;
	cout << map.blueValue << endl;




    
    return 0;
}