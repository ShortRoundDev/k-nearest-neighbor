#include <stdio.h>
#include <stdlib.h>
#include <math.h>

	//types
#define APARTMENT 	0
#define HOUSE 		1
#define UNKNOWN		2

#define NUMLISTINGS 28

	/*training data*/
struct listing{
	int squareFeet;
	int bedrooms;
	int type;
	int distance;
} listings[28] = {
	/*apartments*/
	{567, 	0, 	APARTMENT, -1},
	{607, 	0, 	APARTMENT, -1},
	{718, 	1, 	APARTMENT, -1},
	{719, 	1, 	APARTMENT, -1},
	{728, 	1, 	APARTMENT, -1},
	{739, 	1, 	APARTMENT, -1},
	{1116, 	2, 	APARTMENT, -1},
	{1135, 	2, 	APARTMENT, -1},
	{1137, 	2, 	APARTMENT, -1},
	{1179, 	2, 	APARTMENT, -1},
	{1285, 	3, 	APARTMENT, -1},
	{1289, 	3, 	APARTMENT, -1},
	/*houses*/
	{5711, 5, HOUSE, -1},
	{3111, 4, HOUSE, -1},
	{2864, 3, HOUSE, -1},
	{3155, 4, HOUSE, -1},
	{1892, 3, HOUSE, -1},
	{4127, 5, HOUSE, -1},
	{1966, 3, HOUSE, -1},
	{3963, 4, HOUSE, -1},
	{2482, 3, HOUSE, -1},
	{2482, 3, HOUSE, -1},
	{2500, 4, HOUSE, -1},
	/*unknowns*/
	{3528, 4, UNKNOWN, -1},	//HOUSE
	{2830, 4, UNKNOWN, -1}, //HOUSE
	{877,  2, UNKNOWN, -1}, //APARTMENT
	{1010, 2, UNKNOWN, -1}, //APARTMENT
	{1178, 3, UNKNOWN, -1}};//APARTMENT

	//returns an array of listings with all known-type listings
struct listing *getNeighbors(int *sizeOfReturn);
	//returns an array of listings with distances calculated
struct listing *getDistance(int unknownRooms, int unknownArea, int numOfNeighbors, struct listing *toCheck);
	//returns an array of listings sorted by distance
struct listing *bubbleSort(struct listing *toSort, int size);
	//swaps two listings
void swap(struct listing *a, struct listing *b);
	//returns the type based on the most common type of k-nearest neighbors
int getType(struct listing *toCheck, int k);

int main(void){	

		//iterate along listings
	for(int i = 0; i < NUMLISTINGS; i++){
			//get k nearest neighbor if the listing is not known
		if(listings[i].type == UNKNOWN){
				//initialize to non-useable number
			int sizeOfArray = -1;

			struct listing *neighbors = getNeighbors(&sizeOfArray);									
			neighbors = getDistance(listings[i].bedrooms, listings[i].squareFeet, sizeOfArray, neighbors);			
			neighbors = bubbleSort(neighbors, sizeOfArray);

			int type = getType(neighbors, 5);
				
				//print which listing we're talking about here
			printf("\t%d:", i);
			if(type == APARTMENT)
				printf("\tThis is an apartment!\n");
			else if(type == HOUSE)
				printf("\tThis is a house!\n");
			free(neighbors);
		}		
	}

}

	//returns all non-unknown listings
struct listing *getNeighbors(int *sizeOfReturn){	
		//array to fill and return
	struct listing *toReturn;
	int size = 0;

		//get size of toReturn
	for(int i = 0; i < NUMLISTINGS; i++){
		if(listings[i].type != UNKNOWN)
			size++;			
	}
		//pass size back to caller
	*sizeOfReturn = size;
	toReturn = malloc(sizeof(struct listing) * size);
	int toReturnIndex = 0;

		//fill up toReturn
	for(int i = 0; i < NUMLISTINGS; i++){		
		if(listings[i].type != UNKNOWN){
			toReturn[toReturnIndex] = listings[i];			
			toReturnIndex++;
		}
	}	
	return toReturn;
}

	//returns array with distances from the unknown being checked
struct listing *getDistance(int unknownRooms, int unknownArea, int numOfNeighbors, struct listing *toCheck){
		//copy original list of neighbors
	struct listing *toReturn = toCheck;

		//calculate distance for each neighbor
	for(int i = 0; i < numOfNeighbors; i++){

		int dRooms = toCheck[i].bedrooms - unknownRooms;						
		int dArea  = toCheck[i].squareFeet - unknownArea;			

		toReturn[i].distance = sqrt(dRooms * dRooms + dArea * dArea);
	}		
	return toReturn;
}	

	//Bubblesort because I don't feel like implementing quicksort for a simple test
struct listing *bubbleSort(struct listing *toSort, int size){
		//array of sorted neighbors to be returned
	struct listing *toReturn;
		//boolean check to see if more sorting is required
	int stillSorting = 0;

	do{
			//default to false
		stillSorting = 0;
			//size-1 to so i+1 can be accessed
		for(int i = 0; i < size-1; i++){
				//sort by distance			
			if(toSort[i].distance > toSort[i+1].distance){				
				swap(&toSort[i], &toSort[i+1]);			
				stillSorting = 1;
			}
		}
	}while(stillSorting);

	toReturn = toSort;	
	return toReturn;
}

	//swaps two listings
void swap(struct listing *a, struct listing *b){	
	struct listing tmp = *a;
	*a = *b;
	*b = tmp;	
}

	//returns type based on K-nearest neighbor
int getType(struct listing *toCheck, int k){
	
	int apartment = 0, house = 0;
		//count the types of k nearest neighbors
	for(int i = 0; i < k; i++){
		if(toCheck[i].type == APARTMENT)
			apartment++;
		if(toCheck[i].type == HOUSE)
			house++;		
	}

		//return the type of most k nearest neighbors
	return apartment > house ? APARTMENT : HOUSE;

}