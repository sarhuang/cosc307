/* Name: Sarah Huang
 * Date: 4/11/22
 * Program: city_map.cpp 
 * Purpose: Create a graph of intersections and use Dijkstra's algorithm to find the shortest path from light[0, 0] to light[max_street, max_avenue].
 */


#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include "city_map.hpp"
using namespace std;


//Constructor - create City_Map from city file on standard input
City_Map::City_Map(){		
	int st;					//Street
	int av;					//Avenue
	double x_coor;			//X
	double y_coor;			//Y
	double green_st;		//Duration of green light for street
	double green_av;		//Duration of green light for avenue
	Intersection* inter;	//Intersection
	vector < vector <Intersection*> > intersections;	//intersections[street][avenue] = specific intersection
	set <int> num_streets;		//Unique streets
	set <int> num_avenues;		//Unique avenues

	Road_Segment* rad;		//Road segment between two intersections
	Intersection* inter2;	//Intersection #2
	double dist;			//Distance
	bool even_street, odd_street, even_avenue, odd_avenue;	//True or false if a street and avenue is even or odd
	list <Intersection*>::iterator lit;	


	//Read file of intersections through standard input
	while(!cin.fail()){
		inter = new Intersection;
		cin >> st >> av >> x_coor >> y_coor >> green_st >> green_av;

		if(!cin.fail()){
			inter->street = st;
			num_streets.insert(st);
			inter->avenue = av;
			num_avenues.insert(av);

			inter->x = x_coor;
			inter->y = y_coor;
			inter->green[0] = green_st;
			inter->green[1] = green_av;
			inter->best_time = -1;
			all.push_back(inter);
		}
	}

	//Organize all intersections based on street and avenue
	intersections.resize(num_streets.size(), vector<Intersection*>(num_avenues.size()));
	for(lit = all.begin(); lit != all.end(); lit++){
		intersections[(*lit)->street][(*lit)->avenue] = *lit;		
	}

	//Create edges
	for(int s = 0; s < (int)intersections.size(); s++){	
		if(s % 5 == 0){
			//two-way
			even_street = true;
			odd_street = true;
		}
		else if(s % 2 == 0){
			//east to west
			even_street = true;
			odd_street = false;
		}
		else{
			//west to east
			even_street = false;
			odd_street = true;
		}
		
		//Determine what direction street or avenue goes in
		//Create road segments for each intersection
		for(int a = 0; a < (int)intersections[s].size(); a++){
			inter = intersections[s][a];
			
			//First and last intersections
			if(s == 0 && a == 0)
				first = inter;
			if(s == (int)intersections.size()-1 && a == (int)intersections[s].size()-1)
				last = inter;


			if(a % 5 == 0 || a == (int)intersections[s].size()-1){
				//two-way
				even_avenue = true;
				odd_avenue = true;
			}
			else if(a % 2 == 0){
				//South to north
				even_avenue = true;
				odd_avenue = false;
			}
			else{
				//North to south
				even_avenue = false;
				odd_avenue = true;
			}

			//Left
			if((a-1) >= 0 && odd_street){
				inter2 = intersections[s][a-1];
				rad = new Road_Segment;
				rad->type = STREET;
				rad->number = s;
				dist = sqrt(pow(inter->x - inter2->x, 2) + pow(inter->y - inter2->y, 2));
				rad->distance = dist;
				rad->from = inter;
				rad->to = inter2;
				inter->adj.push_back(rad);
			}
			//Right
			if((a+1) < (int)intersections[s].size() && even_street){
				inter2 = intersections[s][a+1];
				rad = new Road_Segment;
				rad->type = STREET;
				rad->number = s;
				dist = sqrt(pow(inter->x - inter2->x, 2) + pow(inter->y - inter2->y, 2));
				rad->distance = dist;
				rad->from = inter;
				rad->to = inter2;
				inter->adj.push_back(rad);
			}
			//Up
			if((s+1) < (int)intersections.size() && even_avenue){
				inter2 = intersections[s+1][a];
				rad = new Road_Segment;
				rad->type = AVENUE;
				rad->number = a;
				dist = sqrt(pow(inter->x - inter2->x, 2) + pow(inter->y - inter2->y, 2));
				rad->distance = dist;
				rad->from = inter;
				rad->to = inter2;
				inter->adj.push_back(rad);
			}
			//Down
			if((s-1) >= 0 && odd_avenue){
				inter2 = intersections[s-1][a];
				rad = new Road_Segment;
				rad->type = AVENUE;
				rad->number = a;
				dist = sqrt(pow(inter->x - inter2->x, 2) + pow(inter->y - inter2->y, 2));
				rad->distance = dist;
				rad->from = inter;
				rad->to = inter2;
				inter->adj.push_back(rad);
			}
		}
	}
}




//Implements Dijkstra's shortest path algorithm from node first to node last
double City_Map::Dijkstra(int avg_best_worst){
	list <class Road_Segment*>::iterator rit;	
	list <Intersection*>::iterator lit;
	


	//Insert the first node
	first->backedge = NULL;
	first->best_time = 0.0;
	bfsq.insert(pair<double, Intersection*>(first->best_time, first));
	
	double time = 0.0;
	
	while(!bfsq.empty()){
		//Remove first node
		Intersection* n = bfsq.begin()->second;
		bfsq.erase(bfsq.begin());


		//Traverse n's adjacency list
		for(rit = n->adj.begin(); rit != n->adj.end(); rit++){
			cout << "to->best = " << (*rit)->to->best_time << endl;
			

			//if(!bfsq.empty()){
				//n->bfsq_ptr = bfsq.find((*rit)->from->best_time);
				time = (*rit)->from->best_time;
			

			if((*rit)->to->best_time == -1 || time > (*rit)->to->best_time){
				
				if((*rit)->to->best_time != -1){
					bfsq.erase(time);
					//bfsq.erase((*rit)->to->best_time);
				}
				(*rit)->to->best_time = (*rit)->from->best_time;

				//Time for red light
				if(avg_best_worst != 'B'){
					if((*rit)->type == STREET)
						(*rit)->to->best_time = (*rit)->from->best_time + (*rit)->to->green[1];
					else
						(*rit)->to->best_time = (*rit)->from->best_time + (*rit)->to->green[0];
				}
				
				(*rit)->to->backedge = *rit;
				(*rit)->to->best_time += (*rit)->distance * 120;
				bfsq.insert(pair<double, Intersection*>((*rit)->to->best_time, (*rit)->to));
			}
				
	


/*	for(lit = all.begin(); lit != all.end(); lit++){
		 multimap <double, Intersection *>::iterator ded = (*lit)->bfsq_ptr;

		for(ded = bfsq.begin(); ded != bfsq.end(); ded++){
			cout << "(" << ded->second->street << ", " << ded->second->avenue << ")	 = " << ded->first << "	";
		}
		cout << endl;
	}
	cout << endl;
*/		
		}//for
	} //while
	
	Intersection* inter_temp;
	class Road_Segment* road_temp;
	

	//Starting from last intersection, backtrack to the beginning to find path
	inter_temp = last;
	road_temp = last->backedge;
	path.push_back(road_temp);

	
	inter_temp = road_temp->from;
	road_temp = inter_temp->backedge;

	while(road_temp != NULL){
		path.push_back(road_temp);
		inter_temp = road_temp->from;
		road_temp = inter_temp->backedge;
	}
	path.sort();


	return last->best_time;
}
