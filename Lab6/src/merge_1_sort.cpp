/* Name: Sarah Huang
 * Date: 3/27/22
 * Program: merge_1_sort.cpp
 * Purpose: Mimic a merge sort
 *			Base case 1: If the vector's size is one, then it's already sorted -- return it.
 *			Base case 2: If the vector's size is two, then sort it with an if statement, and maybe a swap.
 *			Otherwise, split the vector into two equal parts, and recursively sort both parts.
 *			Now that the two parts are sorted, merge the parts together into a sorted vector.
 */ 

#include <iostream>
#include <vector>
#include "sorting.hpp"
using namespace std;


void recursive_sort(vector <double> &v, vector <double> &temp, int start, int size, int print){
	double t;	//Temporary storage for an element
	int count;	

	//Print B tag
	if(size > 1){
		printf("B:     %d     %d   ", start, size);
		for(unsigned int index = 0; index < v.size()-1; index++){
			printf("%.2f ", v[index]);
		}
		printf("%.2f\n", v[v.size()-1]);
	}

	//Base case 1: already sorted
	if(size == 1){
		return;
	}
	//Base case 2: manually swap
	else if(size == 2){	
		if(v[start] > v[start+1]){
			t = v[start];
			v[start] = v[start+1];
			v[start+1] = t;
		}
	}
	//Split vector in half and recursively sort both parts
	else{
		recursive_sort(v, temp, start, size/2, print);
		recursive_sort(v, temp, start + size/2, size-size/2, print);
	}


	//Double check that all elements are sorted
	if(size > 2){
		count = 0;
		while(count < size){
			for(int i = 0; i < size-1; i++){
				if(v[start+i] > v[start+i+1]){
					t = v[start+i];
					v[start+i] = v[start+i+1];
					v[start+i+1] = t;
				}
			}
			count++;
		}
	}


	//Store in temp and push it back to v
	if(start == 0 && size == (int) v.size()){
		vector <double> one;
		vector <double> two;
		
		//Store the split vector into 2 actual different vectors
		for(unsigned int a = 0; a < v.size(); a++){
			if(a < v.size()/2)
				one.push_back(v[a]);
			else
				two.push_back(v[a]);
		}	

		//Push back all elements into temp until there is none left
		while(one.size() != 0 || two.size() != 0){	
			//Last element remaining in second half
			if(two.size() == 0 && one.size() != 0){
				temp.push_back(one[0]);
				one.erase(one.begin());
			}
			//Last element remaining in first half
			else if(one.size() == 0 && two.size() != 0){
				temp.push_back(two[0]);
				two.erase(two.begin());
			}
			//First half's element < second half's element, don't delete second half element
			else if(one[0] < two[0]){
				temp.push_back(one[0]);
				one.erase(one.begin());
			}
			//First half's element > second half's element, don't delete first half element
			else{
				temp.push_back(two[0]);
				two.erase(two.begin());
			}
		}
			
		//Copy all elements of temp --> v
		v.clear();
		for(unsigned int j = 0; j < temp.size(); j++){
			v.push_back(temp[j]);
		}
	}

	//Print result
	printf("E:     %d     %d   ", start, size);
	for(unsigned int index = 0; index < v.size()-1; index++){
		printf("%.2f ", v[index]);
	}
	printf("%.2f\n", v[v.size()-1]);
}



void sort_doubles(vector <double> &v, bool print){
	vector <double> temp;
	recursive_sort(v, temp, 0, (int) v.size(), print);

	//Print vector one final time
	printf("                 ");
	for(unsigned int i = 0; i < v.size()-1; i++){
		printf("%.2f ", v[i]);
	}
	printf("%.2f\n", v[v.size()-1]);
}
