#include <iostream>
#include "my_quadtree.h"
#include <sstream>
#include <fstream>

using namespace std;
//hw of aleyna beste ozhan
int main()
{
	string line = "";
	string line1="";
	ifstream file("cities.txt");
	int x_bound,y_bound, xcoord,ycoord;
	string name_of_city;
	getline(file,line1);
	istringstream s(line1);
	s>>x_bound;
	s>>y_bound;
	My_QuadTree city_tree(x_bound,y_bound);
	while(getline(file,line))
	{
		istringstream ss (line)	;
		ss>>name_of_city;
		ss>>xcoord;
		ss>>ycoord;
		city_tree.insert(xcoord,ycoord, name_of_city);

	}
	//printing
	city_tree.print();

	//searching
	
	ifstream file2("queries.txt");
	string line2="";
	
	while(getline(file2,line2))
	{
		istringstream sss(line2);
		int i; 
		vector<int> vec; 
		while(sss >> i) 
		{ 
		vec.push_back(i); 
 
		if(sss.peek() == ',') 
			sss.ignore();
		}
		city_tree.search(vec[0],vec[1],vec[2]);
	
	}



	return 0;
}