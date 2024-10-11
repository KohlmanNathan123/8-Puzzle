#include <iostream>
#include <queue>
#include <vector>

//Struct to represent node on tree for A*
struct Node {
	Node* parent;
	int state[3][3];
	int depth;
	//x and y coordinates for 0
	int x;
	int y;
};

//Method to output state
void printState(int state[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << state[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//Method to check if in goal state
bool checkGoal(int curState[3][3], int goal[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (curState[i][j] != goal[i][j]) {
				return false;
			}
		}
	}
	return true;
}

//Method to create new node
Node* makeNode(Node* p, int s[3][3], int d, int sX, int sY) {
	//Make new node
	Node* n = new Node;

	//Initialize node attributes to parameters
	n->parent = p;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			n->state[i][j] = s[i][j];
		}
	}
	n->depth = d;
	n->x = sX;
	n->y = sY;

	return n;
}

//Method to find number of correctly placed tiles for heuristic function
int checkCorrectSquares(int state[3][3], int goal[3][3]) {
	int correctSq = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (state[i][j] == goal[i][j]) {
				correctSq++;
			}
		}
	}
	return correctSq;
}

//Method to search closed list to ensure no loops occur in search
bool inClosed(int state[3][3], std::vector<Node*> closed) {
	for (int i = 0; i < closed.size(); i++) {
		if (checkGoal(state, closed[i]->state)) {
			return true;
		}
	}
	return false;
}

//Method to search through tree of puzzle states for goal state
void search(int start[3][3], int goal[3][3]) {
	std::cout << "Start State: " << std::endl;
	printState(start);

	//Find position of 0 in the start state
	int startX;
	int startY;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (start[i][j] == 0) {
				startY = i;
				startX = j;
			}
		}
	}

	//Initialize root node
	Node* startNode = makeNode(NULL, start, 0, startX, startY);

	//Create FIFO queue for A*
	std::queue<Node*> q;
	q.push(startNode);

	//List of nodes already visited
	std::vector<Node*> closed;

	//While loop until no more successors or solution is found
	bool solNotFound = true;

	//Establish a maximum number of moves before terminating and make counter
	int numSteps = 0;
	int stepMax = 20000;
	while (solNotFound) {
		//front of queue and add successors to queue
		Node* curNode = q.front();
		q.pop();
		numSteps++;

		//Add curNode to the closed list
		closed.push_back(curNode);

		
		if (numSteps == stepMax) {
			std::cout << "Current State: " << std::endl;
			printState(curNode->state);
			std::cout << "Number of moves reached max of: " << numSteps << std::endl;
			std::cout << "Solution not found" << std::endl << std::endl;
			solNotFound = false;
		}
		

		//If Goal is found, break from search
		if (checkGoal(curNode->state, goal)) {
			std::cout << "Goal state found" << std::endl;
			printState(curNode->state);
			std::cout << "Number of moves needed: " << numSteps << std::endl;
			std::cout << std::endl;
			solNotFound = false;
		}

		//Add at most 4 children to the queue for left, right, up, and down
		//Check the number of correct squares and add the successor with the most
		int correctSq[4] = { -1,-1,-1,-1 };
		Node* candidates[4];

		//check up
		if (curNode->y != 0) {
			int temp[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					temp[i][j] = curNode->state[i][j];
				}
			}
			//Swap the 0 on the current node with the number above it
			int curX = curNode->x;
			int curY = curNode->y;
			std::swap(temp[curY][curX], temp[curY - 1][curX]);

			correctSq[0] = checkCorrectSquares(temp, goal);

			//If temp has not been visited, add to candidate list

			Node* successor = makeNode(curNode, temp, curNode->depth + 1, curX, curY - 1);
			candidates[0] = successor;
		}

		//check down
		if (curNode->y != 2) {
			int temp[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					temp[i][j] = curNode->state[i][j];
				}
			}
			//Swap the 0 on the current node with the number below it
			int curX = curNode->x;
			int curY = curNode->y;
			std::swap(temp[curY][curX], temp[curY + 1][curX]);

			correctSq[1] = checkCorrectSquares(temp, goal);

			Node* successor = makeNode(curNode, temp, curNode->depth + 1, curX, curY + 1);
			candidates[1] = successor;
		}

		//check left
		if (curNode->x != 0) {
			int temp[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					temp[i][j] = curNode->state[i][j];
				}
			}
			//Swap the 0 on the current node with the number left of it
			int curX = curNode->x;
			int curY = curNode->y;
			std::swap(temp[curY][curX], temp[curY][curX - 1]);

			correctSq[2] = checkCorrectSquares(temp, goal);


			Node* successor = makeNode(curNode, temp, curNode->depth + 1, curX - 1, curY);
			candidates[2] = successor;

		}

		//check right
		if (curNode->x != 2) {
			int temp[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					temp[i][j] = curNode->state[i][j];
				}
			}
			//Swap the 0 on the current node with the number right of it
			int curX = curNode->x;
			int curY = curNode->y;
			std::swap(temp[curY][curX], temp[curY][curX + 1]);

			correctSq[3] = checkCorrectSquares(temp, goal);

			Node* successor = makeNode(curNode, temp, curNode->depth + 1, curX + 1, curY);
			candidates[3] = successor;
		}

		//Select the move that creates the most correct squares and add it to the queue
		int curMax = correctSq[0];
		int index = 0;
		for (int i = 1; i < 4; i++) {
			if (correctSq[i] > curMax) {
				curMax = correctSq[i];
				index = i;
			}
		}
		//Add most desirable candidate first
		//If most desirable candidate is already in closed list, add other valid candidates to queue
		if (!inClosed(candidates[index]->state, closed)) {
			q.push(candidates[index]);
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (correctSq[i] >= 0 && index != i) {
					if (!inClosed(candidates[i]->state, closed)) {
						q.push(candidates[i]);
					}
				}
			}
		}
	}

}

//Method to get a non-duplicate number to make random puzzle configurations
int checkDup(int p[3][3]) {
	int r;
	bool inPuzzle;
	//Until r is not in the puzzle, continually generate random numbers
	while (true) {
		r = rand() % 9;
		inPuzzle = false;

		//Check r against numbers already in puzzle
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (r == p[i][j]) {
					inPuzzle = true;
				}
			}
		}
		//Break and return r if it was not in the puzzle
		if (!inPuzzle) {
			break;
		}
	}
	return r;
}

int main() {
	//Initialize state to check if goal has been reached
	int goalState[3][3] = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 0} };

	int startState[3][3] = {
		{4, 3, 0},
		{7, 1, 6},
		{5, 2, 8} };

	int startState2[3][3] = {
	{0, 8, 5},
	{3, 6, 7},
	{2, 1, 4} };

	//Generate 3 random puzzles
	int p1[3][3];
	int p2[3][3];
	int p3[3][3];
	//Generate a start state for the puzzle
	srand(time(NULL));

	//Generate first puzzle
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int nextNum = checkDup(p1);
			p1[i][j] = nextNum;
		}
	}
	
	//Generate second puzzle
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int nextNum = checkDup(p2);
			p2[i][j] = nextNum;
		}
	}

	//Generate third puzzle
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int nextNum = checkDup(p3);
			p3[i][j] = nextNum;
		}
	}

	//Hand permuted search
	std::cout << "Puzzle with known solution 1: " << std::endl;
	search(startState, goalState);

	std::cout << "Puzzle with known solution 2: " << std::endl;
	search(startState2, goalState);

	//Randomly generated puzzles
	std::cout << "First puzzle: " << std::endl;
	search(p1, goalState);

	
	std::cout << "Second puzzle: " << std::endl;
	search(p2, goalState);


	std::cout << "Third puzzle: " << std::endl;
	search(p3, goalState);
	
}