/* Name: Sarah Huang
 * Date:4/3/22
 * Program: maze_solve.cpp
 * Purpose: Create a graph of the maze given the number of rows, columns, and walls.
 *			Find a solution using Depth First Search.
 *
	Each cell of the maze is a node
	Each cell has four links - up, down, left, right
	Cell is on top row of maze, NO UP LINK
	Cell is on bottom row of maze, NO DOWN LINK
	Cell is on left column of maze, NO LEFT 
	Cell is on right column of maze, NO RIGHT
	Cell link is in the list of walls, NO LINK
*/

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <stdlib.h>
#include <algorithm>
using namespace std;


class Node{
	public:
		int visit;			//The number of times a node is visited (>1 is bad)
		vector <int> edges; //Adjacency list, holding the # of nodes connected to this node
};


class Graph{
	public:
		vector <Node *> nodes;	//All of the nodes
		stack <int> visited;	//The order the cells are visited = the path
		void DFS(int index);	//Depth First Search 
		void Print();			//Print a path solution
};


void Graph::DFS(int index){
	Node *n;
	vector <int>::iterator it;
	
	//We are currently on node n
	n = nodes[index];
	visited.push(index);
	n->visit++;

	//Back out if we stepped on node n again
	if(n->visit > 1) {
		visited.pop();
		return;
	}

	//Go through the possible directions
	for(unsigned int i = 0; i < n->edges.size(); i++){
		//We have reached the end
		if(visited.top() == nodes.size()-1) return;
		
		DFS(n->edges[i]);
	}
	//We have reached the end
	if(visited.top() == nodes.size()-1) return;
	//Or we have to keep backing out because this path was bad
	visited.pop();
}

void Graph::Print(){
	vector <int> path;	//The final solution

	//Transfer the answer from the stack to a vector to print in the right order
	while(!visited.empty()){
		path.push_back(visited.top());
		visited.pop();
	}	
	for(int i = path.size()-1; i >= 0; i--){
		cout << "PATH " << path[i] << endl;
	}
}



int main(){
	Graph g;			//Graph object
	string s;			//String temporary holder
	size_t row, col;	//Row, column
	int n1, n2;			//Wall between cell_1 and cell_2
	Node *n;			//Node/cell in graph
	vector <int>::iterator vit;	//Used for finding n1 and n2 in n[i]->edges
		
	//Read in rows and columns
	cin >> s;
	if(s != "ROWS") { cerr << "Bad graph\n"; exit(1); }
	cin >> row;
	cin >> s;
	if(s != "COLS") { cerr << "Bad graph\n"; exit(1); }
    cin >> col;
	cout << "ROWS " << row << " COLS " << col << endl;


	//Create all nodes
	for(int i = 0; i < row * col; i++){
		n = new Node;
		n->visit = 0;
		g.nodes.push_back(n);
	}

	//Connect all nodes together
	for(unsigned int i = 0; i < g.nodes.size(); i++){
		//Right
		if((i+1) % col != 0)
			g.nodes[i]->edges.push_back(i+1);
	
		//Down
		if(i+col < g.nodes.size())
			g.nodes[i]->edges.push_back(i+col);

		//Up
		if(int(i-col) >= 0)
			g.nodes[i]->edges.push_back(i-col);

		//Left
		if((i-1) >= 0 && i%col != 0)
			g.nodes[i]->edges.push_back(i-1);
	}
	
	//Create walls
	while(!cin.fail()){
		cin >> s >> n1 >> n2;
		if(!cin.fail()){
			if (s != "WALL") { cerr << "Bad graph\n"; exit(1); }	
			cout << s << " " << n1 << " " << n2 << endl;

			//Find and delete the link between the two read-in cells
			vit = find(g.nodes[n1]->edges.begin(), g.nodes[n1]->edges.end(), n2);
			if(vit != g.nodes[n1]->edges.end())
				g.nodes[n1]->edges.erase(vit);

			vit = find(g.nodes[n2]->edges.begin(), g.nodes[n2]->edges.end(), n1);
			if(vit != g.nodes[n2]->edges.end())
				g.nodes[n2]->edges.erase(vit);
		}
	}
	
	g.DFS(0);
	g.Print();
	return 0;
}

