/* Name: Sarah Huang
 * Date: 4/25/22
 * Program: worddice.cpp
 * Purpose: Use network flow to figure out which given dice to use to spell specific words.
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <deque>
#include <algorithm>

using namespace std;


class Edge{
	public:
		class Node* to;
		class Node* from;
		Edge *reverse;	//The opposite of a regular edge (goes in direction of sink -> source)
		int original;
		int residual;
};



class Node{
	public:
		Node();		
		int id;					//Node #0 -> Node #X
		string type;			//SOURCE, dice name, word letter, or SINK
		vector <bool> letters;	//26 units long, true if it has that letter
		int visited;			//0=node not visited in BFS, 1=yes
		vector <Edge*> adj;			
		Edge* backedge;			//The edge that led to this node, different than reverse
};

//Node constructor
Node::Node(){
	visited = 0;
	letters.resize(26, false);
	backedge = NULL;
}



class Graph{
	public:
		vector <Node*> nodes;
		int BFS();
		int canSpell();				//Can you spell the word? 0=no, 1=yes
		vector <int> spellingID;	//Stores the ids of the dice used
		void deleteHalfGraph();		//erase all edges between dice and words, edges between words and sink, delete words and sink nodes
		int minNodes;				//the sum of source and dice nodes
};

int Graph::BFS(){
	deque <Node*> que;
	Node* n;
	Edge* e;

	int sizeCap = 1;
	vector <int> leftovers;
	vector <int>::iterator vit;

	//Clear out the backedges and visited fields
	for(unsigned int i = 0; i < nodes.size(); i++){
		nodes[i]->backedge = NULL;
		nodes[i]->visited = 0;

		if(i >= 1 && i < minNodes)
			leftovers.push_back(nodes[i]->id);
	}


	que.push_back(nodes[0]);
	while(!que.empty()){
		n = que.front();
		que.pop_front();

		//SOURCE and the word nodes
		if(n->type == "SOURCE" || (n->id >= minNodes && n->id < nodes.size()-1)){
			for(unsigned int j = 0; j < n->adj.size(); j++){
				if(n->adj[j]->original == 1 && n->adj[j]->to->visited != 1){
					n->adj[j]->to->visited = 1;
					n->adj[j]->to->backedge = n->adj[j];
					n->adj[j]->to->backedge->from = n->adj[j]->from;
					n->adj[j]->to->backedge->to =  n->adj[j]->to;
					que.push_back(n->adj[j]->to);
				}
			}
		}


		//Dice nodes 
		else if(n->type != "SINK"){
//			cout << "				type = " << n->type << endl;
//			cout << "               adj size = " << n->adj.size() << endl;
			
		//	cout << "					";
		//	for(unsigned y = 0; y < n->adj.size(); y++){
		//		cout << n->adj[y]->to->id << ", ";
		//	}
		//	cout << endl;
			
//			cout << "				size cap: " << sizeCap << endl;

			bool updateCap = true;
			for(unsigned int a = 0; a < leftovers.size(); a++){
				int nodeid = leftovers[a];
		
				if(updateCap && (nodes[nodeid]->adj.size() == sizeCap || nodes[nodeid]->adj.size() == 0))
					updateCap = false;
			}
			if(updateCap){
				sizeCap++;
			}
			else if(n->adj.size() != 0){
				bool allSame = false;
				for(unsigned int b = 0; b < n->adj.size()-1; b++){
//					cout << n->adj[b]->to->type << "	vs		" << n->adj[b+1]->to->type << endl;
					
					if(n->adj[b]->to->type == n->adj[b+1]->to->type)
						allSame = true;
					else{
						allSame = false;
						break;
					}
				}
				if(allSame)
					sizeCap = n->adj.size();
			}

//			cout << "						 new size cap: " << sizeCap << endl;



			if(n->adj.size() == sizeCap){
				vit = find(leftovers.begin(), leftovers.end(), n->id);
				if(vit != leftovers.end())
					leftovers.erase(vit);

				for(unsigned int j = 0; j < sizeCap; j++){
					if(n->adj[j]->original == 1 && n->adj[j]->to->visited != 1){
						n->adj[j]->to->visited = 1;
						n->adj[j]->to->backedge = n->adj[j];
						n->adj[j]->to->backedge->from = n->adj[j]->from;
						n->adj[j]->to->backedge->to =  n->adj[j]->to;
						que.push_back(n->adj[j]->to);

//						cout << "pushed " << n->id << "'s " << n->adj[j]->to->type << endl;
					}
				}
			}
			
			/*
			else{
				bool updateCap = true;

				for(unsigned int a = 0; a < leftovers.size(); a++){
					int nodeid = leftovers[a];
					
					if(updateCap && nodes[nodeid]->adj.size() == sizeCap)
						updateCap = false;
				}

				if(updateCap){
					sizeCap++;
				}
			}*/


			/*
			for(unsigned int j = 0; j < n->adj.size(); j++){
				if(n->adj[j]->original == 1 && n->adj[j]->to->visited != 1){
					n->adj[j]->to->visited = 1;
					n->adj[j]->to->backedge = n->adj[j];
					n->adj[j]->to->backedge->from = n->adj[j]->from;
					n->adj[j]->to->backedge->to =  n->adj[j]->to;
					que.push_back(n->adj[j]->to);
				}
			}*/
		}
	}

	//If the path makes it to the end (the SINK)
	if(nodes[nodes.size()-1]->visited == 1){
		return 1;
	}
	return 0;
}


//Can the word be spelled using the given dice?
int Graph::canSpell(){
	Edge* e;
	int index = 69; //Match entry in adj with *to

	while(BFS()){
		e = nodes[nodes.size()-1]->backedge;
		spellingID.push_back(e->from->backedge->from->id);

		//source->dice, dice->word, word->sink
		for(int steps = 0; steps < 3; steps++){
			//Figure out which edge in adj we're changing
			for(int i = e->from->adj.size()-1; i >= 0; i--){
				if(e->from->adj[i]->to->type == e->to->type){
					index = i;
				}
			}

//			cout << e->from->adj[index]->from->type << "	-->	";
//			cout << e->from->adj[index]->to->type << endl;

			
			//Swap
			e->from->adj[index]->original = 0;
			e->from->adj[index]->residual = 1;
			e->from->adj[index]->reverse->original = 1;
			e->from->adj[index]->reverse->residual = 0;

			nodes[e->to->id]->adj.push_back(e->from->adj[index]->reverse);		
			e = e->from->adj[index]->from->backedge;
		}
//		cout << endl;
	}
	
	//Clear the spellingID for to store final dice
	spellingID.clear();


	for(int j = minNodes; j < (int) nodes.size()-1; j++){
		//Can't make a word
		if(nodes[j]->adj[0]->residual != 1){		
			return 0;
		}

		//Use edge from adj of word node that has an original of 1
		for(unsigned int i = 0; i < nodes[j]->adj.size(); i++){
			if(nodes[j]->adj[i]->original == 1){
		//		cout << "pushing " << nodes[j]->adj[i]->to->id << endl;
				spellingID.push_back(nodes[j]->adj[i]->to->id - 1);
			}
		}
	}

	//Printing
//	cout << spellingID.size() << endl;
	for(unsigned int i = 0; i < spellingID.size()-1; i++){
		cout << spellingID[i] << ",";
	}
	cout << spellingID[spellingID.size()-1] << ": ";
	return 1;
}




//Delete the word and sink nodes to substitute it with new word
void Graph::deleteHalfGraph(){
	Edge* e;
	Edge* rev;
	
	//Delete word and sink nodes	
	nodes.erase(nodes.begin() + minNodes, nodes.end());

	//Delete dice adjacent edges
	nodes[0]->adj.erase(nodes[0]->adj.begin(), nodes[0]->adj.end());
	nodes[0]->visited = 0;
	nodes[0]->backedge = NULL;

	//Reset source and dice edges and reverse edges
	for(unsigned int j = 1; j < nodes.size(); j++){
		nodes[j]->adj.erase(nodes[j]->adj.begin(), nodes[j]->adj.end());
		nodes[j]->visited = 0;
		nodes[j]->backedge = NULL;

		e = new Edge;
		rev = new Edge;
		
		e->to = nodes[j];
		e->from = nodes[0];
		e->original = 1;
		e->residual = 0;
		
		rev->to = nodes[0];
		rev->from = nodes[j];
		rev->original = 0;
		rev->residual = 1;
		
		e->reverse = rev;
		rev->reverse = e;
		nodes[0]->adj.push_back(e);
	
	}
	spellingID.clear();
}




int main(int argc, char* argv[]){
		(void) argc;
		vector <string> dice;
		vector <string> words;
		string temp;
		ifstream ifs;

		Graph g;
		Node* n;
		Edge* e;
		Edge* rev;
		unsigned int i, j;

		unsigned int counter = 0;
		char letter;
		int index;


		//Reading and storing dice
		ifs.open(argv[1]);
		while(getline(ifs, temp)){
			dice.push_back(temp);
		}
		ifs.close();

		//Reading and storing words
		ifs.open(argv[2]);
		while(getline(ifs, temp)){
			words.push_back(temp);
		}
		ifs.close();



		//Create source node
		n = new Node;
		n->id = 0;
		n->type = "SOURCE";
		g.nodes.push_back(n);


		//Create nodes from dice
		for(i = 0; i < dice.size(); i++){
			n = new Node;
			n->id = i+1;
			n->type = dice[i];
			for(j = 0; j < dice[i].size(); j++){
				n->letters[dice[i][j] - 'A'] = true;
			}
			g.nodes.push_back(n);	
		}
		g.minNodes = g.nodes.size();


		//Create source -> dice edges
		for(i = 1; i < g.nodes.size(); i++){
			e = new Edge;
			rev = new Edge;

			e->to = g.nodes[i];
			e->from = g.nodes[0];
			e->original = 1;
			e->residual = 0;

			rev->to = g.nodes[0];
			rev->from = g.nodes[i];
			rev->original = 0;
			rev->residual = 1;

			e->reverse = rev;
			rev->reverse = e;

			g.nodes[0]->adj.push_back(e);
		//	g.nodes[i]->adj.push_back(rev);
		}


		//Goes through all words and makes a new graph
		while(counter < words.size()){

			//Create nodes from words 
			for(j = 0; j < words[counter].size(); j++){
				n = new Node;
				n->id = g.minNodes + j;
				n->type = words[counter][j];	
				n->letters[words[counter][j] - 'A'] = true;
				g.nodes.push_back(n);
			}

			//Create Dice -> Words edges
			for(int i = 1; i < g.minNodes; i++){
				for(unsigned int k = g.minNodes; k < g.nodes.size(); k++){
					letter = g.nodes[k]->type[0];
					index = letter - 'A';


					if(g.nodes[i]->letters[index] && g.nodes[k]->letters[index]){
						e = new Edge;
						rev = new Edge;

						e->to = g.nodes[k];
						e->from = g.nodes[i];
						e->original = 1;
						e->residual = 0;

						rev->to = g.nodes[i];
						rev->from = g.nodes[k];
						rev->original = 0;
						rev->residual = 1;

						e->reverse = rev;
						rev->reverse = e;


						g.nodes[i]->adj.push_back(e);
		//				g.nodes[k]->adj.push_back(rev);
					}
				}
			}

			//Create sink node
			n = new Node;
			n->id = g.nodes.size();
			n->type = "SINK";
			g.nodes.push_back(n);

			//Create word -> sink edges
			for(i = g.minNodes; i < g.nodes.size()-1; i++){
				e = new Edge;
				rev = new Edge;

				e->from = g.nodes[i];
				e->to = g.nodes[g.nodes.size()-1];
				e->original = 1;
				e->residual = 0;

				rev->to = g.nodes[i];
				rev->from = g.nodes[g.nodes.size()-1];
				rev->original = 0;
				rev->residual = 1;

				e->reverse = rev;
				rev->reverse = e;

				g.nodes[i]->adj.push_back(e);
		//		g.nodes[g.nodes.size()-1]->adj.push_back(rev);
				
			}
		

			//Print result
			if(g.canSpell())
				cout << words[counter] << endl;
			else
				cout << "Cannot spell " << words[counter] << endl;


//			cout << "------------------------------------------------------" << endl;
			g.deleteHalfGraph();
			counter++;
		}
		return 0;
	}
