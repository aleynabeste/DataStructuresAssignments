#include <sstream>
#include <fstream>		
#include "MPQ.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
enum LeftOrRight {LEFT,RIGHT}; //enum is used to indicate side, in the building struct. 

struct building //this struct is gonna be used in the building array
{
	int coord_x;
	LeftOrRight side; //this is an indicator of side of x coordinate. 
	int height;
	int buildingID;
};
struct result //this struct is gonna be used in the result array
{
	int finalHeight;
	int coord;
};

struct myclass { //this is for the sorting algorithm
	bool operator() (building i,building j) 
	{ 
		if(i.coord_x != j.coord_x) //if their x coord are different 
			return (i.coord_x < j.coord_x);
		else //if their x are same then take the taller one 
			return (i.height < j.height);
	}
} myobject;

void updateHeight(int & max, vector<building> &b, vector <result> &re,int& k, int &i) //after each insertion and deletion, we set new max height 
{
	if (re[k-1].coord==b[i].coord_x)
		re[k-1].finalHeight=max;
	else
	{
		re[k].coord=b[i].coord_x;
		re[k].finalHeight=max;
		k++;			
	}
}

int main()
{
	
	ifstream file ("input.txt"); // for reading the  input.txt
	string line="";
	getline(file,line); 
	int buildingNumber = atoi(line.c_str()); // first line building num.
	MPQ<int> myHeap(buildingNumber); // MPQ First, and its integer so I create it like int heap
	vector<building> buildings(buildingNumber*2); //twice the number of buildings because I store every x coordinate separetly 
	int xCordL=0;
	int height=0;
	int xCordR=0;
	int counter = 1; //for label
	int i=0;
	while(getline(file,line))
	{ // getting all information
		
		istringstream ss(line); 
		building temp;
		ss >> xCordL; //first one is left x 
		ss >> height ; //second is height 
		ss>> xCordR; //last is right x
		temp.coord_x=xCordL; //I create a temp building which corresponds to left x 
		temp.side= LEFT;
		temp.height=height;
		temp.buildingID=counter; //id is 1 at the beginning and at every line I increment id by one
		buildings[i]=temp; //I put this temp into buildings array
		i++; //I increment the index because for the same building's right x, I am gonna give a new index in the building array
		building temp2; //this one is the right side of the same building 
		temp2.coord_x=xCordR;
		temp2.side= RIGHT;
		temp2.height=height;
		temp2.buildingID=counter;
		buildings[i]=temp2;

		counter++;
		i++;
	}

	sort(buildings.begin(), buildings.end(), myobject); //sorting function. I sort the buildings array
	int size_initial=(buildingNumber*2)+1;
	vector<result> r(size_initial); 
	int k=1;
	int size=buildings.size(); int keepMax=0;
	i=0;
	while( i < size) //we make a left to right sweep across the city , for building size many times 
	{
		if(i==0 )
		{
			if(buildings[i].coord_x != 0)	// if there isn't any building at x=0
			{
				r[0].coord = 0;
				r[0].finalHeight = 0;
			}
		}
		if(buildings[i].side==LEFT) //if its left side, we should insert it 
		{
			myHeap.insert(buildings[i].height,buildings[i].buildingID); //mpq is used to keep track of which building span the current x coord 
			if(myHeap.GetMax()!=keepMax)
			{
				keepMax=buildings[i].height;
				updateHeight(keepMax, buildings, r,k,i);
			}
		
		}
		else if(buildings[i].side==RIGHT) //if its right side we should remove it 
		{
			myHeap.remove(buildings[i].buildingID);
			if(buildings[i].height==keepMax)
			{
				keepMax=myHeap.GetMax();
				updateHeight(keepMax, buildings, r,k,i); //update the result height after each insert and remove op
			
			}
		}
		i++;
	}
	cout << r[0].coord << " " << r[0].finalHeight << endl; //print the first element seperately
	unsigned int index=1;
	while(index < r.size() )
	{
		if(r[index].finalHeight!=r[index-1].finalHeight ) //if the height of the point at that index is not equal to its preceding index, 
			//because we want to show the points where height changes
		{
			if(r[index].coord != 0 )	
				cout << r[index].coord <<	" " << r[index].finalHeight << endl;
		}
		index++;
	}
	file.close(); //just a precaution
	return 0;

}