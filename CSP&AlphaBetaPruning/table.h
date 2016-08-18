/**
 * @file animation.h
 *
 * Defintion for a class that encapsulates creating animated images from a
 * set of PNG object frames.
 *
 * @author Jack Toole
 * @date Fall 2011
 */
#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;


class table {
	public:

		enum Color {GREEN, BLUE, WHITE };


		class node{
			public :
			int cordinateX;
			int cordinateY;
			int value;
			Color color;
		};

		vector< vector<node> > board;
		int width;
		int depth;
		int blueValue;
		int greenValue;

		table();


		/**
		 * find all possible spots for paraDrop strategy
		 *
		 * @param cordinate of the current cell, the current player.
		 */
		void paraDrop(int x,int y,int turn);

		/**
		 * find all possible spots for paraDrop strategy
		 *
		 * @param cordinate of the current cell, the current player.
		 */
		void M1(int x,int y,int turn);

		/**
		 * find all possible spots for paraDrop strategy
		 *
		 * @param cordinate of the current cell, the current player.
		 */

		vector<table::node> allDropAvailable(); // return the set of nodes that a player could play paradrop on them

		vector<table::node> allM1Available();  // return the set of nodes that a player could play M1 on them

		bool checkNeighbor(int x,int y);
	
		void colorNeighbor(int x,int y,int turn);

		void printTable();

		int getDiffBlue();

		int getDiffGreen();

		table & operator= (table  & source);

};

#endif
