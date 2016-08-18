#include <iostream>
#include <stack>   
#include <queue>       
#include <fstream>
#include <string>
#include <cmath> 
#include <algorithm>   
#include <vector>    
#include <limits>
#include <iomanip>  
#include <cmath>  

using namespace std;

class Node {

  	public:
	  	bool wall;  // true if it's a wall
	  	bool left;
	  	bool right;
	  	bool up;
	  	bool down;
	  	bool isTerminal;
	  	int visited;  // for bfs, if it's visited.
	  	float utility;
	  	char optimal_direction;
	  	int inlist;
	  	int x;
	  	int y;

};

vector< vector<Node> > grid;


float rewards_given=-0.04;
float desired_chance=0.8;
float other_chance=0.1;
float discount_factor=0.99;

void initial(){
	for(int i = 0; i < 6; i++){
		vector<Node> temp;
		for(int j = 0; j < 6; j++){
			Node myNode;
			myNode.wall = false;
	  		myNode.left = true;
	  		myNode.right = true;
	  		myNode.up = true;
	  		myNode.down = true;
	  		myNode.visited = 0;
	  		myNode.utility = 0.0;
	  		myNode.x = i;
	  		myNode.y = j;
	  		myNode.isTerminal = false;
	  		myNode.inlist = 0;
	  		myNode.optimal_direction = '.'; // it means undefined
			temp.push_back(myNode);
		}
		grid.push_back(temp);
	}

	//initialize directions
	for(int i = 0; i < 6; i++){
		grid[0][i].up = false;
	}

	for(int i = 0; i < 6; i++){
		grid[i][0].left = false;
	}

	for(int i = 0; i < 6; i++){
		grid[5][i].down = false;
	}

	for(int i = 0; i < 6; i++){
		grid[i][5].right = false;
	}

	//initialize the initial utilities
	grid[0][1].utility = -1.0;
	grid[1][4].utility = -1.0;
	grid[2][5].utility = 3.0;
	grid[5][0].utility = 1.0;
	grid[5][1].utility = -1.0;
	grid[5][4].utility = -1.0;
	grid[5][5].utility = -1.0;

	//initialize the walls
	grid[1][3].wall = true;
	grid[2][3].wall = true;
	grid[3][3].wall = true;
	grid[5][3].wall = true;

	//initialize terminals
	grid[0][1].isTerminal =  true;
	grid[1][4].isTerminal =  true;
	grid[2][5].isTerminal =  true;
	grid[5][0].isTerminal =  true;
	grid[5][1].isTerminal =  true;
	grid[5][4].isTerminal =  true;
	grid[5][5].isTerminal =  true;

	grid[1][3].isTerminal = true;
	grid[2][3].isTerminal = true;
	grid[3][3].isTerminal = true;
	grid[5][3].isTerminal = true;

	//initialize some directions;
	grid[0][3].down = false;

	grid[1][2].right = false;
	grid[2][2].right = false;
	grid[3][2].right = false;
	grid[5][2].right = false;

	grid[1][4].left = false;
	grid[2][4].left = false;
	grid[3][4].left = false;
	grid[5][4].left = false;

	grid[4][3].up = false;
	grid[4][3].down = false;

}


int one_node_calculation(int i, int j) {

	float before_unity=grid[i][j].utility;

	float left_utility=0.0;
	float right_utility=0.0;
	float up_utility=0.0;
	float down_utility=0.0;

	if (grid[i][j].left==true)
	{
		left_utility=grid[i][j-1].utility;
	}
	else{
		left_utility=grid[i][j].utility;
	}

	if (grid[i][j].right==true)
	{
		right_utility=grid[i][j+1].utility;
	}
	else{
		right_utility=grid[i][j].utility;
	}

	if (grid[i][j].up==true)
	{
		up_utility=grid[i-1][j].utility;
	}
	else{
		up_utility=grid[i][j].utility;
	}

	if (grid[i][j].down==true)
	{
		down_utility=grid[i+1][j].utility;
	}
	else{
		down_utility=grid[i][j].utility;
	}

	float left_compare = discount_factor * (desired_chance * left_utility + other_chance * up_utility + other_chance * down_utility) + rewards_given;
	float right_compare = discount_factor * (desired_chance * right_utility + other_chance * up_utility + other_chance * down_utility) + rewards_given;
	float up_compare = discount_factor * (desired_chance * up_utility + other_chance * left_utility + other_chance * right_utility) + rewards_given;
	float down_compare=discount_factor * (desired_chance * down_utility + other_chance * left_utility + other_chance * right_utility) + rewards_given;

	if(left_compare >= right_compare && left_compare >= up_compare && left_compare >= down_compare){
		grid[i][j].utility=left_compare;
		grid[i][j].optimal_direction = '<';
	}
	if(right_compare >= left_compare && right_compare >= up_compare && right_compare >= down_compare){
		grid[i][j].utility=right_compare;
		grid[i][j].optimal_direction = '>';
	}
	if(up_compare >= left_compare && up_compare >= right_compare && up_compare >= down_compare){
		grid[i][j].utility=up_compare;
		grid[i][j].optimal_direction = '^';
	}
	if(down_compare >= left_compare && down_compare >= right_compare && down_compare >= up_compare){
		grid[i][j].utility=down_compare;
		grid[i][j].optimal_direction = 'v';
	}

	float after_unity=grid[i][j].utility;

	if(  abs(before_unity - after_unity) < 0.0000000001  ){
	// if(before_unity == after_unity){
		// cout << i << endl;
		// cout << j << endl;
		// cout << endl;
		grid[i][j].utility = before_unity;

		return 1;
	}
	else{
		return 0;
	}

}

int BFS(){

	int unchanged=0;

	queue<Node> myqueue;
	grid[3][1].inlist = 1;
	myqueue.push(grid[3][1]);//start point
	Node temp;

	while(!myqueue.empty()){

		temp=myqueue.front();
	    myqueue.pop();

	    int currentX=temp.x;
	    int currentY=temp.y;

	    if(temp.visited == 0){

			unchanged = unchanged + one_node_calculation(currentX, currentY);// calculate how many points haven't been changed;

	    	grid[temp.x][temp.y].visited=1;	    
		    if(currentX+1<6){
		    	if(grid[currentX+1][currentY].inlist==0 && grid[currentX+1][currentY].wall==false  &&  grid[currentX+1][currentY].isTerminal==false){
		    		grid[currentX+1][currentY].inlist=1;
		    		myqueue.push(grid[currentX+1][currentY]);
		    	}
		    }
		    if(currentY+1<6){
		    	if(grid[currentX][currentY+1].inlist==0 && grid[currentX][currentY+1].wall==false  &&  grid[currentX][currentY+1].isTerminal==false){
		    		grid[currentX][currentY+1].inlist=1;
		    		myqueue.push(grid[currentX][currentY+1]);
		    	}
		    }
		    if(currentX-1>=0){
		    	if(grid[currentX-1][currentY].inlist==0 && grid[currentX-1][currentY].wall==false  &&  grid[currentX-1][currentY].isTerminal==false){
		    		grid[currentX-1][currentY].inlist=1;
		    		myqueue.push(grid[currentX-1][currentY]);
		    	}
		    }
		    if(currentY-1>=0){
		    	if(grid[currentX][currentY-1].inlist==0 && grid[currentX][currentY-1].wall==false  &&  grid[currentX][currentY-1].isTerminal==false){
		    		grid[currentX][currentY-1].inlist=1;  
		    		myqueue.push(grid[currentX][currentY-1]);		
		    	}
		    }
		}
	}

	return unchanged;
}

void empty(){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			grid[i][j].inlist = 0;
			grid[i][j].visited = 0;
		}
	}
}

void printGrid(){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			if(grid[i][j].utility * 100==0){
				cout << "0.00 ";
			}
			else{
				cout << setprecision(4) << grid[i][j].utility << " ";			
			}
		}
		cout << endl;
	}
}

void printDirection(){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			cout << grid[i][j].optimal_direction<< " ";

		}
		cout << endl;
	}
}


int main(){

	int count = 0;

	initial();
	// cout << grid[2][5].x << endl;
	// cout << grid[2][5].y << endl;
	// cout << grid[1][3].wall << endl;

	int BFSunchanged=0;

	BFSunchanged = BFS();
	cout << BFSunchanged << endl;

	while(BFSunchanged != 25){
		count++;
		empty();
		// cout << endl;
		// cout << endl;
		// cout << endl;
		// cout << endl;
		BFSunchanged=BFS();
		cout << BFSunchanged << endl;
	}

	printGrid();
	printDirection();

	cout << count << endl;

	

	return 0;

}

