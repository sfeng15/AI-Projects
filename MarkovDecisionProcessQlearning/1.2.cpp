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
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>
using namespace std;

class Node {

  	public:
	  	bool wall;  // true if it's a wall
	  	bool left;
	  	bool right;
	  	bool up;
	  	bool down;
	  	int visited;  // for bfs, if it's visited.
	  	float utility;
	  	char optimal_direction;
	  	int inlist;
	  	int x;
	  	int y;

	  	int scoreN;
	  	float scoreUtility;

	  	float reward_initial;

	  	float leftUtility;
	  	float rightUtility;
	  	float upUtility;
	  	float downUtility;

	  	int leftN;
	  	int rightN;
	  	int upN;
	  	int downN;

	float maxUtility(){
		return max( max( max( leftUtility, rightUtility), upUtility), downUtility);
	}

};

vector< vector<Node> > grid;


float rewards_given=-0.04;
float desired_chance=0.8;
float other_chance=0.1;
float discount_factor=0.99;

int maxStep = 1000000;

int global_x = 3;
int global_y = 1;
float global_N=200;
float global_R=50;

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
	  		myNode.inlist = 0;
	  		myNode.reward_initial=rewards_given;
	  		myNode.optimal_direction = '.'; // it means undefined, using ^ v < > 

	  		myNode.leftUtility = 0.0;
	  		myNode.rightUtility = 0.0;
	  		myNode.upUtility = 0.0;
	  		myNode.downUtility = 0.0;

	  		myNode.leftN = 0;
	  		myNode.rightN = 0;
	  		myNode.upN = 0;
	  		myNode.downN = 0;

	  		myNode.scoreUtility = 0.0;
	  		myNode.scoreN = 0;


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
	// grid[0][1].utility = -1.0;
	// grid[1][4].utility = -1.0;
	// grid[2][5].utility = 3.0;
	// grid[5][0].utility = 1.0;
	// grid[5][1].utility = -1.0;
	// grid[5][4].utility = -1.0;
	// grid[5][5].utility = -1.0;

	//initialize the initial reward_initial
	grid[0][1].reward_initial = -1.0;
	grid[1][4].reward_initial = -1.0;
	grid[2][5].reward_initial = 3.0;
	grid[5][0].reward_initial = 1.0;
	grid[5][1].reward_initial = -1.0;
	grid[5][4].reward_initial = -1.0;
	grid[5][5].reward_initial = -1.0;


	//initialize the walls
	grid[1][3].wall = true;
	grid[2][3].wall = true;
	grid[3][3].wall = true;
	grid[5][3].wall = true;

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


void terminalCalc(int x, int y){
	grid[x][y].scoreN = grid[x][y].scoreN+1;
	grid[x][y].scoreUtility = grid[x][y].reward_initial; 
	// grid[x][y].scoreUtility = grid[x][y].scoreUtility + ( 60.0/(59.0+grid[x][y].scoreN) ) * (grid[x][y].reward_initial + discount_factor * grid[x][y].scoreUtility - grid[x][y].scoreUtility);
	grid[x][y].upN = grid[x][y].scoreN;
	grid[x][y].downN = grid[x][y].scoreN;
	grid[x][y].leftN = grid[x][y].scoreN;
	grid[x][y].rightN = grid[x][y].scoreN;
	grid[x][y].upUtility=grid[x][y].scoreUtility;
	grid[x][y].downUtility=grid[x][y].scoreUtility;
	grid[x][y].leftUtility=grid[x][y].scoreUtility;
	grid[x][y].rightUtility=grid[x][y].scoreUtility;
}

void one_node_direction_update(int x, int y, char direction){
	int new_x=0;
	int new_y=0;

	if(direction=='^'){
		grid[x][y].upN = grid[x][y].upN+1;
		if(grid[x][y].up==true){
			new_x=x-1;
			new_y=y;
			grid[x][y].upUtility = grid[x][y].upUtility + ( 60.0/(59.0+grid[x][y].upN) )* (grid[x][y].reward_initial +discount_factor * grid[x-1][y].maxUtility() - grid[x][y].upUtility);
			
		}
		else{
			new_x=x;
			new_y=y;
			grid[x][y].upUtility = grid[x][y].upUtility + ( 60.0/(59.0+grid[x][y].upN) )* (grid[x][y].reward_initial +discount_factor * grid[x][y].upUtility - grid[x][y].upUtility);		
		}
	}

	if(direction=='v'){
		grid[x][y].downN = grid[x][y].downN+1;
		if(grid[x][y].down==true){
			new_x=x+1;
			new_y=y;
			grid[x][y].downUtility = grid[x][y].downUtility + ( 60.0/(59.0+grid[x][y].downN) )* (grid[x][y].reward_initial +discount_factor * grid[x+1][y].maxUtility() - grid[x][y].downUtility);
		}
		else{
			new_x=x;
			new_y=y;
			grid[x][y].downUtility = grid[x][y].downUtility + ( 60.0/(59.0+grid[x][y].downN) )* (grid[x][y].reward_initial +discount_factor * grid[x][y].downUtility - grid[x][y].downUtility);
		}
	}

	if(direction=='<'){
		grid[x][y].leftN = grid[x][y].leftN+1;
		if(grid[x][y].left==true){
			new_x=x;
			new_y=y-1;
			grid[x][y].leftUtility = grid[x][y].leftUtility + ( 60.0/(59.0+grid[x][y].leftN) )* (grid[x][y].reward_initial +discount_factor * grid[x][y-1].maxUtility() - grid[x][y].leftUtility);
		}
		else{
			new_x=x;
			new_y=y;
			grid[x][y].leftUtility = grid[x][y].leftUtility + ( 60.0/(59.0+grid[x][y].leftN) )* (grid[x][y].reward_initial +discount_factor * grid[x][y].leftUtility - grid[x][y].leftUtility);
		}
		

	}

	if(direction=='>'){
		grid[x][y].rightN = grid[x][y].rightN+1;
		if(grid[x][y].right==true){
			new_x=x;
			new_y=y+1;
			grid[x][y].rightUtility = grid[x][y].rightUtility + ( 60.0/(59.0+grid[x][y].rightN) )* (grid[x][y].reward_initial +discount_factor * grid[x][y+1].maxUtility() - grid[x][y].rightUtility);
			
		}
		else{
			new_x=x;
			new_y=y;
			grid[x][y].rightUtility = grid[x][y].rightUtility + ( 60.0/(59.0+grid[x][y].rightN) )* (grid[x][y].reward_initial +discount_factor * grid[x][y].rightUtility - grid[x][y].rightUtility);
		}
	}

	global_x=new_x;
	global_y=new_y;
}

char get_direction(float leftTemp,float rightTemp,float upTemp,float downTemp){
	char direction='.';
	vector<char> shuffle_vector;
	if (rightTemp > leftTemp  &&  rightTemp > upTemp  && rightTemp > downTemp  ){
		direction='>';
		return direction;
	}
	else if (upTemp > leftTemp  &&  upTemp > rightTemp  && upTemp > downTemp  ){
		direction='^';
		return direction;
	}
	else if (leftTemp > rightTemp  && leftTemp > upTemp  && leftTemp > downTemp  ){
		direction='<';
		return direction;
	}
	else if (downTemp > leftTemp  &&  downTemp > upTemp  && downTemp > rightTemp  ){
		direction='v';
		return direction;
	}
	else {
		/// 4 tie
		if(rightTemp == leftTemp  &&  rightTemp == upTemp  && rightTemp == downTemp){
			shuffle_vector.push_back('^');
			shuffle_vector.push_back('<');
			shuffle_vector.push_back('>');
			shuffle_vector.push_back('v');
		}

		//3 tie
		else if(rightTemp == leftTemp  &&  rightTemp == upTemp  && rightTemp != downTemp){
			shuffle_vector.push_back('^');
			shuffle_vector.push_back('<');
			shuffle_vector.push_back('>');	

		}
		else if(rightTemp == leftTemp  &&  rightTemp != upTemp  && rightTemp == downTemp){

			shuffle_vector.push_back('<');
			shuffle_vector.push_back('>');
			shuffle_vector.push_back('v');
		}
		else if(rightTemp != leftTemp  &&  rightTemp == upTemp  && rightTemp == downTemp){
			shuffle_vector.push_back('^');

			shuffle_vector.push_back('>');
			shuffle_vector.push_back('v');
		}
		else if(rightTemp != leftTemp  &&  leftTemp == upTemp  && leftTemp == downTemp){
			shuffle_vector.push_back('^');
			shuffle_vector.push_back('<');

			shuffle_vector.push_back('v');
		}

		// 2 tie
		else if(rightTemp != leftTemp  &&  leftTemp == upTemp  && leftTemp != downTemp){
			shuffle_vector.push_back('^');
			shuffle_vector.push_back('<');


		}
		else if(rightTemp != leftTemp  &&  leftTemp != upTemp  && leftTemp == downTemp){

			shuffle_vector.push_back('<');

			shuffle_vector.push_back('v');
		}
		else if(rightTemp == leftTemp  &&  leftTemp != upTemp  && leftTemp != downTemp){
	
			shuffle_vector.push_back('<');
			shuffle_vector.push_back('>');
			
		}
		else if(rightTemp != leftTemp  &&  rightTemp == upTemp  && rightTemp != downTemp){
			shuffle_vector.push_back('^');

			shuffle_vector.push_back('>');
			
		}
		else if(rightTemp != leftTemp  &&  rightTemp != upTemp  && rightTemp == downTemp){
	
			
			shuffle_vector.push_back('>');
			shuffle_vector.push_back('v');
		}
		else if(upTemp != leftTemp  &&  upTemp != rightTemp  && upTemp == downTemp){
	
			shuffle_vector.push_back('^');
			shuffle_vector.push_back('v');
			
		}




		random_shuffle ( shuffle_vector.begin(), shuffle_vector.end() );
		direction=shuffle_vector[0];
		return direction;
	}
	
	
}


void computeDirection(){
	char direction='.';
	std::vector<float> myvector ;
	int x= global_x;
	int y= global_y;
	// cout << x << " " << y << endl;
	// cout << grid[x][y].reward_initial << endl;
	if(grid[x][y].reward_initial != rewards_given){
		// cout << "go back start" << endl;



		terminalCalc(x, y);
		if(x==2 && y==5 && grid[2][5].scoreN == 1  ){
			cout << grid[5][0].maxUtility() << endl;
			cout << grid[2][5].maxUtility() << endl;
		}
		x=3;
		y=1;
		global_x=x;
		global_y=y;
		return;
	}

	float leftTemp=grid[x][y].leftUtility;
	float rightTemp=grid[x][y].rightUtility;
	float upTemp=grid[x][y].upUtility;
	float downTemp=grid[x][y].downUtility;

	
	if(grid[x][y].rightN< global_N){
		rightTemp=global_R;
	}
	if(grid[x][y].leftN< global_N){
		leftTemp=global_R;
	}
	if(grid[x][y].upN< global_N){
		upTemp=global_R;
	}
	if(grid[x][y].downN< global_N){
		downTemp=global_R;
	}


	direction=get_direction( leftTemp, rightTemp, upTemp, downTemp);

	
	// cout << direction << endl;
	one_node_direction_update(x, y, direction);
}




void printDirection(){
	char direction;
	for(int x = 0; x < 6; x++){
		for(int y = 0; y < 6; y++){
			float biggest = grid[x][y].maxUtility();
			if (biggest==grid[x][y].leftUtility){
				direction='<';
			}
			else if (biggest==grid[x][y].rightUtility){
				direction='>';
			}
			else if (biggest==grid[x][y].upUtility){
				direction='^';
			}
			else if (biggest==grid[x][y].downUtility){
				direction='v';
			}
			if (0 == grid[x][y].rightN &&  0 == grid[x][y].upN  &&  0 == grid[x][y].downN && 0 ==  grid[x][y].leftN )
			{
				direction = '.';
			}
			cout << direction << " "; 

		}
		cout << endl;
	}
}










int main(){


	initial();

	int count=0;
	while(count < maxStep){
		count=count+1;
		computeDirection();
		

	}
			printDirection();
			cout << grid[5][0].maxUtility() << endl;
			cout << grid[2][5].maxUtility() << endl;

	
	

	return 0;

}

