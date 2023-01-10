/* Name: Sarah Huang
 * Program: cs302-midi.cpp
 * Date: 2/19/22
 * Purpose: Converts Midi-Event Files to and from Midi-Note Files.
 */


#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "cs302-midi.h"
#include <string>
#include <algorithm>

using namespace std;



//Note file --> Event file
void CS302_Midi::nd_to_el(){
	el = new EventList;											//list <Event*>
	list <Event*>::const_iterator evit;							//Iterator for el
	map <int, multimap<int, Event*> > temp;						//Temporary map to store events
	map <int, multimap<int, Event*> >::const_iterator tempit;	//Iterator for temporary map
	multimap <int, Event*>::const_iterator mit;					//Iterator for multimap in temporary map
    multimap <double, ND*>::const_iterator ndit;				//Iterator for already existing nd map

	int key;		//Key for event types (1=OFF, 2=DU, 3=DO, 4=ON)
	int rintStop;	//Note stop time using rint()
	int prev;		//Holds previous time to calculate new time	 
	Event* a;	//Event #1
	Event* b;	//Event #2


	//Loop through existing map 'nd' and convert those to events and store in list 'el'
	for(ndit = nd->begin(); ndit != nd->end(); ndit++){			
		key = rint(ndit->second->start * 480.0);
		rintStop = rint(ndit->second->stop * 480.0);
		prev = 0;

		//If there are the same, ignore the note
		if(key != rintStop){	
			//Convert each 'N' into ON and OFF event
			if(ndit->second->key == 'N'){
				a = new Event;
				a->key = 'O';
				a->v1 = ndit->second->pitch;
				a->v2 = ndit->second->volume;
				temp[key].insert(pair <int, Event*> (4, a));		//on

				b = new Event;
				b->key = 'F';
				b->v1 = ndit->second->pitch;
				b->v2 = 0;
				temp[rintStop].insert(pair <int, Event*> (1, b));	//off
			}
			//Convert each 'D' event into DAMPER UP and DAMPER DOWN
			else if(ndit->second->key == 'D'){
				b = new Event;
				b->key = 'D';
				b->v1 = 1;
				temp[key].insert(pair <int, Event*> (3, b));		//down
			
				a = new Event;
				a->key = 'D';
				a->v1 = 0;
				temp[rintStop].insert(pair <int, Event*> (2, a));    //up
			}


			//Loop through temporary map and inner multimap
			for(tempit = temp.begin(); tempit != temp.end(); tempit++){
				for(mit = tempit->second.begin(); mit != tempit->second.end(); mit++){
					
					//Find if there is a duplicate - lists can hold copies
					evit = find(el->begin(), el->end(), mit->second);
					//No copy
					if(evit == el->end()){
						mit->second->time = tempit->first - prev;
						el->push_back(mit->second);
						prev = tempit->first;
					}
					//There is a copy - delete it
					else if(evit != el->end()){
						el->erase(evit);
						mit->second->time = tempit->first - prev;
						el->push_back(mit->second);
						prev = tempit->first;
					}

				}
			}
		}
	}
}




//Event file --> Note file
void CS302_Midi::el_to_nd(){	
	list <Event*>::const_iterator lit;	//Iterator for el
	vector <ND*> notes(128, NULL);		//Vector to store ONLY notes
	ND* temp;							//Temporary note placeholder
	nd = new NDMap;						//Create a new 'nd' map
	double addTime = 0.0;				//Holds previous time to calculate new time
	ND* damper = new ND;				//Damper variable


	//Loop through the EventList 'el' to translate those elements into notes for 'nd'
	for(lit = el->begin(); lit != el->end(); lit++){
		temp = new ND;

		//Note ON
		if((*lit)->key == 'O'){
			temp->key = 'N';
			temp->pitch = (*lit)->v1;
			temp->volume = (*lit)->v2;
			temp->start = ((*lit)->time / 480.0) + addTime;
			notes[(*lit)->v1] = temp;
			addTime = addTime + ((*lit)->time / 480.0);
		}
		//Note OFF
		else if((*lit)->key == 'F'){
			notes[(*lit)->v1]->stop = ((*lit)->time / 480.0) + addTime;
			addTime = addTime + ((*lit)->time / 480.0);
			nd->insert(pair <double, ND*> (notes[(*lit)->v1]->start, notes[(*lit)->v1]));
		}
		//DAMPER
		else if((*lit)->key == 'D'){
			//Pitch = 1 --> Damper Down
			if((*lit)->v1 == 1){
				damper->key = 'D';
				damper->pitch = 1;
				damper->start = ((*lit)->time / 480.0) + addTime;
				addTime = addTime + ((*lit)->time / 480.0);
			}
			//Pitch = 0 --> Damper Up
			else if((*lit)->v1 == 0){
				damper->stop = ((*lit)->time / 480.0) + addTime;
				addTime = addTime + ((*lit)->time / 480.0);
				nd->insert(pair <double, ND*> (1, damper));
				damper = new ND;
			}
		}
	}
}
