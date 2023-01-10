/* Name: Sarah Huang
 * Date: 2/13/22
 * Program: hashylvania.cpp
 * Purpose: Randomly order people given their names and IDs using hashing.
 */

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


struct Person{
	string name;						//Name of person
	string id;							//ID of person
	unsigned long long personHash;		//Key for Person* map
	unsigned long long personHashMod;	//Key for m entry
};


//Given hashing function
unsigned long long jsp_hash(const string &s){
	size_t i;
	unsigned long long h, t;

	h = 0x6bd300f29f1505ULL;
	for(i = 0; i < s.size(); i++){
		t = h & 0x3ffffffffffffffULL;
		h = (t << 5) ^ h ^ s[i];
	}
	return h;
}



//Find all of the people whose hash modulo the modulus is closest h modulo the modulus
unsigned long long find_closest_hashMod(map <unsigned long long, map <unsigned long long, Person*> > &m, unsigned long long h_mod){
	//Variables
	map <unsigned long long, map <unsigned long long, Person*> >::const_iterator mit_FCHM;	//Iterator for m
	map <unsigned long long, map <unsigned long long, Person*> >::const_iterator temp;		//Iterator for no lower_bound
	unsigned long long above;	//Original lower bound Hash%Modulus
	unsigned long long below;	//hashMod under Hash%Modulus
	bool failure = false;		//If iterator is pointing to end of map

	//Grab the hash%modulus closest to h
	mit_FCHM = m.lower_bound(h_mod);

	//The Hash%Modulus closest to h IF it's less than h
	temp = m.find(mit_FCHM->first);
	if(temp == m.end()){
		mit_FCHM = m.end();
		mit_FCHM--;
		below = mit_FCHM->first;
		return below;
	}
	//SAME as the lower bound
	else if(mit_FCHM->first == h_mod){
		return h_mod;
	}
	//Comparing above and below lower bound to see which is closer
	else{
		above = mit_FCHM->first;
		mit_FCHM--;
		below = mit_FCHM->first;	

		temp = m.find(below);
		if(temp == m.end()){
			failure = true;
		}

		//The one above is closer - FAVOR HIGHER VALUES
		if(above-h_mod <= h_mod-below || failure)
			return above;

		//The one below is closer
		else
			return below;
	}
}


//Now, of those people, find the person whose hash is the smallest hash greater than or equal to h.
//If there is no such person, then choose the person in that collection with the largest hash.
Person* find_closest_person(map <unsigned long long, map <unsigned long long, Person*> > &m, 
		unsigned long long hash, unsigned long long hash_mod){
	
	//Variables
	map <unsigned long long, Person*>::const_iterator mit_FCP;	//Iterator for inside map
	map <unsigned long long, Person*>::const_iterator tem;		//Iterator to check lower_bound
	unsigned long long key;		//Hash key for m
	Person* who;				//Next person in order
	

	//Find the person with the closest hash - no need to do top bottom stuff
	key = find_closest_hashMod(m, hash_mod);
	mit_FCP = m[key].lower_bound(hash);	

	//If lower_bound fails - nothing is greater than h
	tem = m[key].find(mit_FCP->first);
	if(tem == m[key].end()){
		mit_FCP = m[key].end();
		mit_FCP--;
		who = m[key][mit_FCP->first];
	}
	else
		who = m[key][mit_FCP->first];

	return who;
}



int main(){
	vector <Person*> people;															
	map <unsigned long long, map <unsigned long long, Person*> > m;						
	map <unsigned long long, map <unsigned long long, Person*> >::const_iterator mit;
	map <unsigned long long, Person*>::const_iterator mit2;

	string choice;				//Keyword PERSON or ORDER
	string word1;				//Name or Random String
	string word2;				//ID or Modulus
	unsigned long long hashMod;	//Hash%Modulus key for m
	unsigned long long hash;	//Key for inner map
	string nameIdConcat;		//Concatenated Name+Id+RandomString
	unsigned long long modulus;	//Modulus but as a number
	unsigned count;				//Total number of people

	unsigned long long h;		//Hash
	unsigned long long hMod;	//Hash % Modulus
	char hexH[20];				//Hash in hex
	
	Person* first;				//First person
	Person* last;				//Last person	
	Person* p;					//Temp person


	//Read in people until user stops
	cin >> choice;
	do{
		if(choice == "PERSON"){
			cin >> word1 >> word2;	//Name and id
			p = new Person;
			p->name = word1;
			p->id = word2;
			people.push_back(p);
			count = people.size();
		}
		else if(choice == "ORDER"){
			m.clear();
			cin >> word1 >> word2;	//RandomString and modulus
					
			//Store everything in map
			for(unsigned int i = 0; i < people.size(); i++){
				//Calculate hash
				nameIdConcat = people[i]->name + people[i]->id + word1;
				hash = jsp_hash(nameIdConcat);
				people[i]->personHash = hash;

				//Convert modulus to number
				sscanf(word2.c_str(), "%llu", &modulus);  

				//Calculate hash % modulus
				hashMod = hash % modulus;
				people[i]->personHashMod = hashMod;
				
				//Add entry in m for (hash % modulus), hash, person
				m[hashMod][hash] = people[i];
			}
			

			//Calculate h
			h = jsp_hash(word1);
			hMod = h % modulus;		
			first = find_closest_person(m, h, hMod);	//Find the first person
			
			//Delete from map
			m[first->personHashMod].erase(first->personHash);
			if(m[first->personHashMod].size() == 0){
				m.erase(first->personHashMod);
			}
			
			//Calculate new h
			sprintf(hexH, "%llx", h);
			h = jsp_hash(first->id + hexH);
			hMod = h % modulus;
			
			count--;
			while(count > 0){
				//Find person (eventually the last)
				last = find_closest_person(m, h, hMod);
				
				//Delete from map
				m[last->personHashMod].erase(last->personHash);
				if(m[last->personHashMod].size() == 0){
					m.erase(last->personHashMod);
				}

				//Calculate new h
				sprintf(hexH, "%llx", h);
				h = jsp_hash(last->id + hexH);
				hMod = h % modulus;

				count--;
			}


			//If there was only one person
			if(people.size() == 1){
				printf("Last: %s %s\n", first->name.c_str(), first->id.c_str());
				printf("First: %s %s\n", first->name.c_str(), first->id.c_str());
			}
			else{
				printf("Last: %s %s\n", last->name.c_str(), last->id.c_str());
				printf("First: %s %s\n", first->name.c_str(), first->id.c_str());
			}
		}
		cin >> choice;
	}while(!cin.eof());
	return 0;
}
