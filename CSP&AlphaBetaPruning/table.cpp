#include "table.h"
using namespace std;

table::table(){
	width=6;
	depth=6;
	blueValue=0;
	greenValue=0;
	
	for(int i=0;i<6;i++){
		vector<node> nodes;
		for(int j=0;j<6;j++){
			node empty;
			empty.cordinateX=i;
			empty.cordinateY=j;
			empty.value=0;
			empty.color=WHITE;
			nodes.push_back(empty);
		}
		board.push_back(nodes);
	}
}



table & table::operator= (table  & source){
	if (this != &source)
	{

		for(int i=0;i<6;i++){
			for(int j=0;j<6;j++){
				this->board[i][j]=source.board[i][j];
			}
		}
		this->width=source.width;
		this->depth=source.depth;
		this->blueValue=source.blueValue;
		this->greenValue=source.greenValue;
	}
	return *this;
}

void table::paraDrop(int x,int y,int turn){


	if (turn==-1)//blue piece
	{
		board[x][y].color=BLUE;
		blueValue+=board[x][y].value;;
	}
	else{
		board[x][y].color=GREEN;
		greenValue+=board[x][y].value;;
	}



}


vector<table::node> toVector(set<table::node> myset){

	set<table::node>::iterator it;
	std::vector<table::node> myVector;
	for (it = myset.begin(); it != myset.end(); ++it)
	{
	     myVector.push_back(*it); // Note the "*" here
	}
	return myVector;
}

vector<table::node> table::allDropAvailable(){

	std::vector<table::node> myVector;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			if (board[i][j].color == WHITE && checkNeighbor(i,j) == false)
			{
				myVector.push_back(board[i][j]);
			}
		}
	}

	return myVector;

}

vector<table::node> table::allM1Available(){

	//set<table::node> myM1Set;
	std::vector<table::node> myVector;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			if (board[i][j].color == WHITE && checkNeighbor(i,j) == true)
			{
				myVector.push_back(board[i][j]);
			}
		}
	}

	return myVector;

}

void table::M1(int x,int y,int turn){

	//blue piece turn
	if (turn==-1){
		board[x][y].color=BLUE;
		blueValue+=board[x][y].value;
		colorNeighbor(x,y,turn);
	}
	else{
		board[x][y].color=GREEN;
		greenValue+=board[x][y].value;		
		colorNeighbor(x,y,turn);
	}


}
void table::colorNeighbor(int x,int y,int turn){
	if (turn==-1){
		if (x+1<width && board[x+1][y].color==GREEN){
			board[x+1][y].color=BLUE;
			blueValue+=board[x+1][y].value;
			greenValue-=board[x+1][y].value;		
					
		}
		if (x-1>0&& board[x-1][y].color==GREEN){
			board[x-1][y].color=BLUE;
			blueValue+=board[x+1][y].value;
			greenValue-=board[x+1][y].value;		
		
		}
		if (y+1<depth && board[x][y+1].color==GREEN){
			board[x][y+1].color=BLUE;
			blueValue+=board[x][y+1].value;
			
			greenValue-=board[x][y+1].value;
		}
		if (y-1>0&& board[x][y-1].color==GREEN){
			board[x][y-1].color=BLUE;
			blueValue+=board[x][y-1].value;

			greenValue-=board[x][y-1].value;
		}
	}
	else {
		if (x+1<width && board[x+1][y].color==BLUE){
			board[x+1][y].color=GREEN;
			blueValue+=board[x+1][y].value;
			greenValue-=board[x+1][y].value;
		}
		if (x-1>0&& board[x-1][y].color==BLUE){		
			board[x-1][y].color=GREEN;
			blueValue+=board[x-1][y].value;
			greenValue-=board[x-1][y].value;
		}
		if (y+1<depth && board[x][y+1].color==BLUE){
			board[x][y+1].color=GREEN;
			blueValue+=board[x][y+1].value;
			greenValue-=board[x][y+1].value;
		}
		if (y-1>0&& board[x][y-1].color==BLUE){
			board[x][y-1].color=GREEN;

			blueValue+=board[x][y-1].value;
			greenValue-=board[x][y-1].value;
		}
	}

}	

bool table::checkNeighbor(int x,int y){
		int counterGreen=0;
		int counterBlue=0;
		//true if M1 is available
			if(board[x][y].color==WHITE){
				if (x+1<width){
					if(board[x+1][y].color==GREEN)
						counterGreen++; 
					if (board[x+1][y].color==BLUE)
						counterBlue++;
				}
				if (x-1>0){
					if(board[x-1][y].color==GREEN)
						counterGreen++; 
					if (board[x-1][y].color==BLUE)
						counterBlue++;
				}
				if (y+1<depth){
					if(board[x][y+1].color==GREEN)
						counterGreen++; 
					if (board[x][y+1].color==BLUE)
						counterBlue++;
				}
				if (y-1>0){
					if(board[x][y-1].color==GREEN)
						counterGreen++; 
					if (board[x][y-1].color==BLUE)
						counterBlue++;
				}
			}

			if (counterBlue>=1&&counterGreen>=1)
			{
				return true;
			}
			else
				return false;
}

void table::printTable(){
	cout<<"========================"<<endl;
	for(int i=0;i<6;i++){
		for(int j=0;j<6;j++){
			if(board[i][j].color==GREEN){
				cout<<"G ";
			}
			if(board[i][j].color==BLUE){
				cout<<"B ";
			}
			if(board[i][j].color==WHITE){
				cout<<"  ";
			}	
		}
		cout<<endl;
	}
	cout<<"========================"<<endl;
}

int table::getDiffGreen(){
	return blueValue-greenValue;
}

int table::getDiffBlue(){
	return greenValue-blueValue;
}