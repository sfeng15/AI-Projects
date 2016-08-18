
#include <cstdint>

#include "tree.h"

using namespace std;

Node::Node() {
 
}

Node::Node(int i, table t) {

	turn = i;
	mytable = t;
	level=0;

	parent=NULL; 
	selectPtr=NULL; 

}

Node::~Node() {
	for (int i = 0; i < (int)children.size(); i++)
	{
		delete children[i];
	}
}

Node & Node::operator=(Node & n){
	if (this != &n)
	{
		this->level=n.level;
		this->turn=n.turn;
		this->mytable=n.mytable;
		this->children=n.children;
		this->parent=n.parent;
		this->selectPtr=n.selectPtr;
	}
	return *this;  

}



void Node::addDropChildren() {
	if(level>1){
		return;
	}

	vector<table::node> dropXY= mytable.allDropAvailable();
	for(int i=0;i<(int)dropXY.size();i++){
		Node * child=new Node(turn,mytable);
		child->parent=this;
		child->level=this->level+1;
		child->mytable.paraDrop(dropXY[i].cordinateX,dropXY[i].cordinateY,turn*(-1));
		children.push_back(child);
	}
	


}

void Node::addM1Children() {
	if(level>1){
		return;
	}	

	vector<table::node> M1XY= mytable.allM1Available();
	for(int i=0;i<(int)M1XY.size();i++){
		Node * child=new Node(turn,mytable);
		child->parent=this;
		child->level=this->level+1;
		child->mytable.M1(M1XY[i].cordinateX,M1XY[i].cordinateY,turn*(-1));
		children.push_back(child);
	}

}


void Node::addChildren() {
	this->addDropChildren();
	this->addM1Children();
	for(int i=0;i<(int)children.size();i++){
		children[i]->addChildren();
	}
	this->select();
	


}



Node * Node::selectGreen(){

	int max = -10000;
	Node * tmp = NULL;

	for (int i = 0; i < (int)children.size(); i++)
	{
		int diff = children[i]->mytable.getDiffGreen(); // get the difference;
		if (diff >= max)
		{
			max = diff;
			tmp = children[i];
		}
	}

	return tmp;

}

Node * Node::selectBlue(){

	
	int max = -10000;
	Node * tmp = NULL;;

	for (int i = 0; i < (int)children.size(); i++)
	{
		int diff = children[i]->mytable.getDiffBlue(); // get the difference;
		if (diff >= max)
		{
			max = diff;
			tmp = children[i];
		}
	}

	return tmp;

}

void  Node::select(){

	
	if (turn == -1) // blue
	{
		selectPtr =selectBlue();
	}
	else { // green
		selectPtr=selectGreen();
	}

}



