# include <unordered_map>
# include <iostream>
#include <sstream>
#include <fstream>	
#include "Header.h"//I use a modified heap class
using namespace std ;



struct seat//this is to store the reservations, used in get seat function
{
	string groupname;
	
	string rowname;
	int index;
	seat(string g_name ="",  string row="", int i=0)
	{
		groupname=g_name;
		rowname=row;
		index=i;
	}

};
unordered_map <string, seat> names; //this unordered map maps the names to seats so that I find them in constant time



struct row //this is used in stadium data structure. Each group-row pair has this struct and the vector inside it represents the seats 
{
	int size;
	vector<string> column;
	int occupied;
	row(int w): size(w)
	{
		occupied=0;
		for (int s=0; s<w; s++)
		{
			column.push_back("---"); //initially all seats are empty
		}
	}
	row(){}

};
void print(unordered_map <string, unordered_map<string, row>> &s, ofstream& output) //complexity is O(B*R*C)
{
	output<<endl;
	for (auto const& i : s)   //first iterate over block names
	{
		output << i.first << endl;
		output <<"~~~~~~~" << endl;
		for (auto const& j : i.second) //then iterate over row names
		{
			output << j.first<< ": " ;
			for(int k=0; k<j.second.size; k++) //and iterate over every index in a row
			{
				output << j.second.column[k] << " "; 
			
			}
			output << endl;
		}
		output << "=======" << endl << endl;
	}

}
void reserve_seat(unordered_map <string, unordered_map<string, row>> &s, 
	string name, string groupname, string rowName,int index,unordered_map <string, 
	BinaryHeap> &h, int max_col_num ,ofstream& output) //complexity is O(logB)
{
	auto k= names.find(name);
	if(k == names.end())//if there is no such name made reseration before
	{

		if(s[groupname][rowName].column[index]=="---")//if the desired seat is not taken
		{
			s[groupname][rowName].column[index]=name.substr(0,3);
			s[groupname][rowName].occupied++;
			names[name]= seat(groupname,rowName,index);//for the names map, I record the new reservation name
			auto it = h.find(rowName);//from the heaps map, I fing the one with corresponding rownma
			it->second.update( groupname,max_col_num, "increment");//this is where it takes O(logB)
			output << name << " has reserved " << groupname << " " << rowName<<"-"<<index<<endl;
		}
		else
			output << name << "could not reserve a seat!" << endl;


	}
	else
		output << name << "could not reserve a seat!" << endl;
	

}
void reserve_seat_by_row(unordered_map <string, unordered_map<string, row>> &s, 
	string name, string rowName, unordered_map <string, BinaryHeap> &h,  
	int max_col_num,ofstream& output) //complexity is O(logB+C)
{
	auto k= names.find(name);
	if(k == names.end())//if there is no reservation for that name before
	{
	   auto it = h.find(rowName);
	   if(it != h.end())
	   {
		   item b=  it->second.findMin();//constant
		   it->second.deleteMin();//logB+C: I first get the min, update it and I put it back
		   it->second.insert(b.value+1, b.label); //logB
		   bool check=true;
		   for(int i=0; i< max_col_num; i++)//C
		   {
				if(s[b.label][rowName].column[i]=="---" && check==true) //I am updating the stadium data structure here
				{
					s[b.label][rowName].column[i]=name.substr(0,3);
					s[b.label][rowName].occupied++;
					names[name]= seat(b.label,rowName,i);//constant
					output << name << " has reserved " << b.label << " " << rowName << "-" << i << " by emptiest block" << endl;
					check=false;
				}
			
		   }
		   if(check==true)
			  output << name << " could not reserve a seat!" << endl;

	   }
	   else
		   output << name << " could not reserve a seat!" << endl;
	}
	else 
		output << name << " could not reserve a seat!" << endl;

}
void get_seat(string name,ofstream& output)//O(1)
{
	auto k= names.find(name);
	if(k != names.end())//if there is a name like that
	{
		output << "Found that "<< name << " has a reservation in " << k->second.groupname << " " << k->second.rowname <<"-"<< k->second.index << endl;
	
	}
	else
		output <<"There is no reservation made for " << name <<"!" << endl;
	
}
void cancel_reservation(string name,unordered_map <string, unordered_map<string, row>> &s,  
	unordered_map <string, BinaryHeap> &h, int max_col_num,ofstream& output)//O( LogB )
{
	auto k= names.find(name);
	if(k != names.end())//if there is such reservation
	{
		string gname=k->second.groupname;
		 string rowname=k->second.rowname;
		 int index=k->second.index;
		 s[gname][rowname].column[index]="---"; //I make the seat empty again
		 s[gname][rowname].occupied--;
		 names.erase(name);//and erase the name
		 auto it = h.find(rowname);//constant
		 if(it != h.end())
		 {
			 it->second.update(gname,max_col_num, "decrement"); //this is the operation of logB, I update my heap
			 output << "Cancelled the resevation of " << name << endl;
		 }
		 else
			 output <<"Could not cancel the reservation for " << name<< "; no reservation found!" << endl;
	
	}
	else 
		output <<"Could not cancel the reservation for " << name<< "; no reservation found!" << endl;
}

int main()
{
	ofstream output;
	output.open("output.txt");
	unordered_map <string, unordered_map<string, row>> stadium; //This is how I create the data structure for the stadium
	unordered_map <string, BinaryHeap> heaps; //this is another map where I map each row name to a heap
	
	vector <string> categoryNames;
	vector <string>	rowNames;
	ifstream file ("input.txt");
	string line="", line2="", line3="";
	getline(file,line);
	string categoryname;
	istringstream ss(line); 
	while(ss>>categoryname) //first I store the groupnames
	{
		categoryNames.push_back(categoryname);
	}
	getline(file,line2);
	string rowname;
	istringstream ss2(line2); 
	while(ss2>>rowname) //I store the rownames as well
	{
		rowNames.push_back(rowname);
	}
	getline(file,line3);
	int colnum;
	istringstream ss3(line3); 
	ss3>>colnum; //this is the num of seats in one row
	
	for (int i=0; i<categoryNames.size(); i++)
	{
		
		for (int j=0; j<rowNames.size(); j++)
		{
			stadium[categoryNames[i]][rowNames[j]]=row(colnum); //then I map each group/row pair to a row struct with colnum many seats 

		}
	
	}
	for (int k=0; k<rowNames.size();k++)
	{
		heaps[rowNames[k]]=BinaryHeap(categoryNames.size());
		for(int b=0; b<categoryNames.size(); b++ )
		{
			
			heaps[rowNames[k]].insert(0,categoryNames[b]); //then for each row, I create a heap and insert every category name to it with initially 0 value
		}

	}
	
	
	string operation,name, name2, groupname, rowName;
	int index;
	
	while(getline(file,line))
	{
		if(line!="")//to avoid empty lines
		{
			istringstream s(line);
			s>>operation;
			if(operation=="reserve_seat") //since its given that input file format is static, I basically check the matches. 
			{
				s>>name;
				s>>groupname;
				s>>rowName;
				s>>index; //in this case, I am given every info to reserve seat
				reserve_seat(stadium,name,groupname, rowName,index, heaps,colnum, output);

			}
			if(operation=="reserve_seat_by_row")
			{
				s>>name;
				s>>rowName;//I am just given the name of row
				reserve_seat_by_row(stadium,name, rowName, heaps, colnum,output);

			}
			if (operation=="print")
			{
				print(stadium,output);
			}
			if (operation=="get_seat")
			{
				s>>name;
				get_seat(name,output);
			}
			if (operation=="cancel_reservation")
			{
				s>>name2;
				cancel_reservation(name2,stadium,heaps, colnum,output);
			}
		}

	}
	output.close();
	file.close();


}