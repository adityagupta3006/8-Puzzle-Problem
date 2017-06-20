// 8_puzzle_problem.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <algorithm>


using namespace std;
vector<int> initial,goal_state;
int z;

class tree {									// defining a class
private:									
	struct treeNode {
		treeNode* nextNode1;					// creating next nodes for possible expansion
		treeNode* nextNode2;					// creating next nodes for possible expansion
		treeNode* nextNode3;					// creating next nodes for possible expansion
		treeNode* nextNode4;					// creating next nodes for possible expansion 
		treeNode* parentNode;					// creating parent node of the tree
		vector<int> state;					
		int path_cost;
		int zeroPosition;
		int distance;
	};
	vector<int> initial, goal_state;
	vector<treeNode*> stack;
	treeNode* rootNode;							// creating a root node
	int heuristic;
	bool success;

public:										
	tree() {
		rootNode = NULL;
	}

	bool isNull() const { return rootNode == NULL; }
	void addNode(treeNode*);							// defining addNode function
	void makeRootNode(vector<int>);						// defining makeRootNode function
	void process();										// defining process function
	void userInput();									// defining userInput function
	void makeMove(treeNode*, treeNode*, char);			// defining makeMove function
	void print(treeNode*);								// defining print function
	void pv(treeNode*);									// defining pv function
	void checkSuccess(treeNode*);						// defining checkSuccess function
	int misplacedTiles(treeNode*);						// defining misplacedTiles function
	void sort();										// defining sort function
};

void tree::userInput()													 // creating a function to take user input
{
	cout << "Enter elements between 0 to 8 for initial state: " << endl; // input the initial state
	for (int i = 0; i <= 8; i++) {
		cin >> z;														 // storing user input in a variable z
		if (z >= 0 && z <= 8) {
			initial.push_back(z);										 // storing the input in initial state
		}
	}
	cout << endl;

	cout << "Enter elements between 0 to 8 for goal state: " << endl; //input the goal state
	for (int i = 0; i <= 8; i++) {
		cin >> z;															// storing user input in a variable z
		if (z >= 0 && z <= 8) {
			goal_state.push_back(z);										// storing the input in goal state
		}
	}
	cout << endl;
}

int tree::misplacedTiles(treeNode* t) {					// creating a function to calculating heuristic value
	int path_cost = 0;

	for (int i = 0; i<(signed)t->state.size(); i++) {
		if (t->state[i] != goal_state[i]) {				// checking if the corresponding elements are same 
			path_cost++;								// incrementing the path cost if the condition is true
		}
	}

	return path_cost;
}

void tree::process() {
	makeRootNode(initial);
	success = false;									// setting the value 
	treeNode* current_node;								// creating a structure
	heuristic = 1;

	int counter = 0;
	while (stack.size() > 0) {							
		current_node = stack.front();					// moving the stack pointer to current_node state
		stack.erase(stack.begin());						// erasing the data at beginning state of the stack
		addNode(current_node);							// calling the addNode fuction

		if (current_node->distance > counter) {
			counter = current_node->distance;			// assigning the value of distance to counter if the condtition is true
			cout<< counter << " nodes" << endl;			// to get the output value of n
		}
	}
}

void tree::makeRootNode(vector<int> state) {						// creating a function to make a root node
	treeNode* treeNode1 = new treeNode;								// creating an empty tree node
	for (int i = 0; i<(signed)state.size(); i++)
		treeNode1->state.push_back(state[i]);						// assigning the states of the initial node to tree node 1
		treeNode1->path_cost = misplacedTiles(treeNode1);			// assigning the heuristic value
		treeNode1->distance = 1;									
		treeNode1->nextNode1 = NULL;								// setting the next nodes as null
		treeNode1->nextNode2 = NULL;
		treeNode1->nextNode3 = NULL;
		treeNode1->nextNode4 = NULL;
		treeNode1->parentNode = NULL;								// setting the parent node as null

	for (int i = 0; i<(signed)state.size(); i++) {
		if (state[i] == 0) {										// finding the position of zero
			treeNode1->zeroPosition = i;
		}
	}

	if (isNull()) {													// checking the null condition
		rootNode = treeNode1;										// assigning data to rootNode
		stack.push_back(rootNode);									// copying the data of rootNode to the stack
	}
}


void tree::makeMove(treeNode* current_node, treeNode* t, char direction) {		//creating a function which expands a node
	int temporary;
	int	zeroPosition;
	bool wrongMove = false;

	for (int i = 0; i<(signed)current_node->state.size(); i++) {
		if (current_node->state[i] == 0) {										// finding the position of zero
			zeroPosition = i;													
		}
		t->state.push_back(current_node->state[i]);								// assigning corresponding elements to t
	}
	t->zeroPosition = zeroPosition;												// assigning the value to zeroPosition 

	if (current_node->distance > 2) {
		if (zeroPosition == current_node->parentNode->zeroPosition) {			// checking if a wrong move is taken
			wrongMove = true;
		}
	}

	switch (direction) {														// switch case to take a move
	case 'u':																	// move up
		if ((zeroPosition > 2) && (!wrongMove)) {
			temporary = t->state[zeroPosition];									// swapping
			t->state[zeroPosition] = t->state[zeroPosition - 3];				// moving the position 
			t->state[zeroPosition - 3] = temporary;
		}
		else
			t->state.erase(t->state.begin(), t->state.end());					// erasing the data in state
		break;
	case 'r':																	// move right
		if ((zeroPosition % 3 != 2) && (!wrongMove)) {
			temporary = t->state[zeroPosition];									// swapping
			t->state[zeroPosition] = t->state[zeroPosition + 1];				// moving the position 
			t->state[zeroPosition + 1] = temporary;								
		}
		else
			t->state.erase(t->state.begin(), t->state.end());					// erasing the data in state
		break;
	case 'd':																	// move down
		if ((zeroPosition < 6) && (!wrongMove)) {
			temporary = t->state[zeroPosition];									// swapping
			t->state[zeroPosition] = t->state[zeroPosition + 3];				// moving the position 
			t->state[zeroPosition + 3] = temporary;
		}
		else
			t->state.erase(t->state.begin(), t->state.end());					// erasing the data in state
		break;
	case 'l':																	// move left
		if ((zeroPosition % 3 != 0) && (!wrongMove)) {
			temporary = t->state[zeroPosition];									// swapping
			t->state[zeroPosition] = t->state[zeroPosition - 1];				// moving the position 
			t->state[zeroPosition - 1] = temporary;			
		}
		else
			t->state.erase(t->state.begin(), t->state.end());					// erasing the data in state
		break;
	}
}

void tree::addNode(treeNode* current_node) {
	treeNode* treeNode1 = new treeNode;
	makeMove(current_node, treeNode1, 'u');
	treeNode1->distance = current_node->distance + 1;
	treeNode1->path_cost = misplacedTiles(treeNode1) + treeNode1->distance;
	treeNode1->nextNode1 = NULL; treeNode1->nextNode2 = NULL; treeNode1->nextNode3 = NULL; treeNode1->nextNode4 = NULL;
	treeNode1->parentNode = current_node;

	treeNode* treeNode2 = new treeNode;
	makeMove(current_node, treeNode2, 'r');
	treeNode2->distance = current_node->distance + 1;
	treeNode2->path_cost = misplacedTiles(treeNode2) + treeNode2->distance;
	treeNode2->nextNode1 = NULL; treeNode2->nextNode2 = NULL; treeNode2->nextNode3 = NULL; treeNode2->nextNode4 = NULL;
	treeNode2->parentNode = current_node;

	treeNode* treeNode3 = new treeNode;
	makeMove(current_node, treeNode3, 'd');
	treeNode3->distance = current_node->distance + 1;
	treeNode3->path_cost = misplacedTiles(treeNode3) + treeNode3->distance;
	treeNode3->nextNode1 = NULL; treeNode3->nextNode2 = NULL; treeNode3->nextNode3 = NULL; treeNode3->nextNode4 = NULL;
	treeNode3->parentNode = current_node;

	treeNode* treeNode4 = new treeNode;
	makeMove(current_node, treeNode4, 'l');
	treeNode4->distance = current_node->distance + 1;
	treeNode4->path_cost = misplacedTiles(treeNode4) + treeNode4->distance;
	treeNode4->nextNode1 = NULL; treeNode4->nextNode2 = NULL; treeNode4->nextNode3 = NULL; treeNode4->nextNode4 = NULL;
	treeNode4->parentNode = current_node;

	if ((current_node->nextNode1 == NULL) && (treeNode1->state.size() > 0)) {
		current_node->nextNode1 = treeNode1;
		if (!success) stack.push_back(current_node->nextNode1);
	}
	else
		delete treeNode1;
	if ((current_node->nextNode2 == NULL) && (treeNode2->state.size() > 0)) {
		current_node->nextNode2 = treeNode2;
		if (!success) stack.push_back(current_node->nextNode2);
	}
	else
		delete treeNode2;
	if ((current_node->nextNode3 == NULL) && (treeNode3->state.size() > 0)) {
		current_node->nextNode3 = treeNode3;
		if (!success) stack.push_back(current_node->nextNode3);
	}
	else
		delete treeNode3;
	if ((current_node->nextNode4 == NULL) && (treeNode4->state.size() > 0)) {
		current_node->nextNode4 = treeNode4;
		if (!success) stack.push_back(current_node->nextNode4);
	}
	else
		delete treeNode4;

	sort();
	checkSuccess(current_node);
}

void tree::sort() {																// create a function to sort the stack data
	treeNode* temporary;
	for (int i = 0; i<(signed)stack.size() - 1; i++) {
		for (int j = 0; j<(signed)stack.size() - 1; j++) {
			if (stack[j]->path_cost > stack[j + 1]->path_cost) {
				temporary = stack[j];											// sorting by bubble sort
				stack[j] = stack[j + 1];
				stack[j + 1] = temporary;
			}
		}
	}
}

void tree::checkSuccess(treeNode* current_node) {								// creating a function checkSuccess
	bool	pass1 = true, pass2 = true,	pass3 = true, pass4 = true;
	for (int i = 0; i<(signed)goal_state.size(); i++) {
		if (current_node->nextNode1 != NULL) {
			if (current_node->nextNode1->state[i] != goal_state[i])				// matching the corresponding state values
				pass1 = false;
		}
		else
			pass1 = false;
		if (current_node->nextNode2 != NULL) {
			if (current_node->nextNode2->state[i] != goal_state[i])			    // matching the corresponding state values
				pass2 = false;
		}
		else
			pass2 = false;
		if (current_node->nextNode3 != NULL) {
			if (current_node->nextNode3->state[i] != goal_state[i])				// matching the corresponding state values
				pass3 = false;
		}
		else
			pass3 = false;
		if (current_node->nextNode4 != NULL) {
			if (current_node->nextNode4->state[i] != goal_state[i])				// matching the corresponding state values
				pass4 = false;
		}
		else
			pass4 = false;
	}
	if (pass1) {																// checking if the goal state is reached or not
		success = true;			
		cout << "The 8-puzzle problem has been solved" << endl;
		print(current_node->nextNode1);
	}
	else if (pass2) {															// checking if the goal state is reached or not
		success = true;
		cout << "The 8-puzzle problem has been solved" << endl;
		print(current_node->nextNode2);
	}
	else if (pass3) {															// checking if the goal state is reached or not
		success = true;
		cout << "The 8-puzzle problem has been solved" << endl;
		print(current_node->nextNode3);
	}
	else if (pass4) {															// checking if the goal state is reached or not
		success = true;
		cout << "The 8-puzzle problem has been solved" << endl;
		print(current_node->nextNode4);
	}

	if (success == true) {														// clearing stack data
		while (!stack.empty())
			stack.erase(stack.begin());
	}
}

void tree::pv(treeNode* t) {
	for (int i = 0; i<(signed)t->state.size(); i++) {
		cout << t->state[i] << " ";
		if (i % 3 == 2)
			cout << endl;
	}
	cout << t->path_cost << " " << t->distance << endl;					// print the heuristic and distance
	cout << endl;
}

void tree::print(treeNode* t) {												// printint the states
	vector<treeNode*> toPrint;
	while (t != NULL) {
		toPrint.push_back(t);												// pushing the data to toPrint
		t = t->parentNode;
	}
	cout << endl << "Initial State" << endl;	
	for (int j = toPrint.size() - 1; j >= 0; j--) {
		for (int i = 0; i<(signed)toPrint[j]->state.size(); i++) {
			if (toPrint[j]->state[i] == 0)									// what should be printed in place of zero
				cout << "0 ";
			else
				cout << toPrint[j]->state[i] << " ";
			if (i % 3 == 2) cout << endl;
		}
		cout << endl;
	}
	cout << "Goal State reached by expanding ";
}
int main() {

	tree eightPuzzle;						// creating object of class tree
	eightPuzzle.userInput();				// calling a function to accept user input
	eightPuzzle.process();					// calling the process function 

}