/* Name: Sarah Huang
 * Date: 3/27/22
 * Program: quick_2_sort.cpp
 * Purpose: Mimic quick sort.
 *			1. Choosing the partition as the median of the first, middle and last element
 *			2. Increment the left pointer until it points to an element ≥ the pivot (or it points to v[start+size]).
 *			3. Decrement the right pointer until it points to an element ≤ the pivot. (or it points to v[start]).
 *			4. Determine if done.
 *				If not, swap the two elements, increment the left pointer, decrement right pointer, and repeat.
 *			5. When done, swap the pivot in elements v[start] with the last element of the left set. 
 *			6. Then recursively sort the left and right sets, omitting the pivot, since it is already in the correct place.
 */

#include <iostream>
#include <vector>
#include "sorting.hpp"
using namespace std;


void recursive_sort(vector <double> &v, int start, int size, int print){
	double first;	//First element in v
	double middle;	//Middle element in v
	double last;	//Last element in v
	double temp;	//Temporary storage for element
	int pivot = 0;	//The pivot's index

	int left;		//Left pointer
	int right;		//Right poitner


	//Print S tag
	if(size > 1){
		printf("S:     %d     %d       ", start, size);
		for(unsigned int index = 0; index < v.size()-1; index++){
			printf("%.2f ", v[index]);
		}
		printf("%.2f\n", v[v.size()-1]);
	}

	//Base case 1: already sorted
	if(size < 2){
		return;
	}
	//Base case 2: no partition, manually sort
	else if(size == 2){
		if(v[start] > v[start+1]){
			temp = v[start];
			v[start] = v[start+1];
			v[start+1] = temp;
		}
		return;
	}
	//Create partition
	else{
		first = v[start];
		last = v[start+size-1];
		middle = v[start+size/2];

		//first is median - already in place
		//if((middle < first && first < last) || (last < first && first < middle)
		
		//middle is median
		if((first < middle && middle < last) || (last < middle && middle < first)){
			temp = v[start+size/2];
			v[start+size/2] = v[start];
			v[start] = temp;
		}
		//last is median
		else if((first < last && last < middle) || (middle < last && last < first)){
			temp = v[start];
			v[start] = v[start+size-1];
			v[start+size-1] = temp;
		}
		//Duplicate case - 1 1 0
		else if(first == middle && first > last){
			temp = v[start+size/2];
			v[start+size/2] = v[start];
			v[start] = temp;
		}
		//Duplicate case - 0 0 1
		else if(first == middle && first < last){
			temp = v[start+size/2];
			v[start+size/2] = v[start];
			v[start] = temp;
		}
		//Duplicate case - 1 0 1
		else if(first == last && first > middle){
			temp = v[start];
			v[start] = v[start+size-1];
			v[start+size-1] = temp;
		}
		//Duplicate case - 0 1 0
		else if(first == last && first < middle){
			temp = v[start];
			v[start] = v[start+size-1];
			v[start+size-1] = temp;
		}
		//Duplicate case - 0 1 1
		else if(middle == last && last > first){
			temp = v[start];
			v[start] = v[start+size-1];
			v[start+size-1] = temp;
		}
		//Duplicate case - 1 0 0
		else if(middle == last && last < first){
			temp = v[start+size/2];
			v[start+size/2] = v[start];
			v[start] = temp;
		}


		//Print M - includes pivot element
		printf("M:     %d     %d  %.2f ", start, size, v[start]);
		for(unsigned int index = 0; index < v.size()-1; index++){
			printf("%.2f ", v[index]);
		}
		printf("%.2f\n", v[v.size()-1]);


		left = start+1;
		right = start+size-1;
		while(left < right){
			if(v[left] >= v[start] && v[right] <= v[start]){
				temp = v[left];
				v[left] = v[right];
				v[right] = temp;

				left++;
				right--;

			}
			while(v[left] < v[start])
				left++;

			while(v[right] > v[start])
				right--;
		}


		//swap the pivot in elements v[start] with the last element of the left set
		temp = v[start];
		v[start] = v[right];
		v[right] = temp;
		pivot = right;


		//Print P label - pivot index
		printf("P:     %d     %d     %d ", start, size, pivot);
		for(unsigned int index = 0; index < v.size()-1; index++){
			printf("%.2f ", v[index]);
		}
		printf("%.2f\n", v[v.size()-1]);


		//Recursively sort left and right sets, omitting the pivot
		recursive_sort(v, start, right-start, print);
		recursive_sort(v, left, start+size-left, print);
	}
}



void sort_doubles(vector <double> &v, bool print){
	recursive_sort(v, 0, (int) v.size(), print);

	//Print out vector one final time
	printf("                     ");
	for(unsigned int i = 0; i < v.size()-1; i++){
		printf("%.2f ", v[i]);
	}
	printf("%.2f\n", v[v.size()-1]);
}
