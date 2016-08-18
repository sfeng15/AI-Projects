#ifndef TREE_H
#define TREE_H


#include <iostream>
#include <vector>
#include <string>


#include "table.h"

using namespace std;


class Node{

	public:
		int level;
		int turn; // -1 blue, 1 green; -1 means blue will make move next
		table mytable; // the big table of the game.
		vector<Node *> children; // the children of the node
		Node * parent; // store the parent of the node
		Node * selectPtr; 

		// functions



		Node(); // constructor

		Node(int i, table t);

		~Node();

		Node & operator=( Node & n);

		void addChildren();

		void addDropChildren();

		void addM1Children();

		Node * selectGreen();

		Node * selectBlue();

		void select();


};

#endif 
