
#include <iostream>
#include <string>
#include <vector>
# include <unordered_map>
using namespace std;

struct item //I am gonna use this in the heap
{
	string label; //group name
	int value; 
	item(int val=0, string l="")
	{
		value=val; 
		label=l;
	};
 //Constructor for struct
};


class BinaryHeap
{
    public:
	BinaryHeap( int c=0 );
	~BinaryHeap();
    bool isEmpty( ) const;
    const item  findMin( ) const;
	void percolateDown( int hole );
    void insert( int x, string l );
    void deleteMin( );
	void update (string group_name, int max_value, string command);

    private:
	unordered_map <string, int> location;
    int  currentSize;
	int capacity;// Number of elements in heap
    item* array;       // The heap array
    void buildHeap( vector<item> Input);
	
    
};
void BinaryHeap::update(string group_name, int max_value, string command)
{//logn
	auto it = location.find(group_name);
	int k=it->second;
	if(array[k].value+1<= max_value && command=="increment")
	{
		array[k].value++;

		int child;
		int tmp = array[ k ].value; // tmp is the item that will  be percolated down
		string tmplabel = array[ k].label;

		int x=0;
		for( ; k * 2 <= currentSize; k = child ) //logB
		{
			x++;
			child = k * 2;
			if( child != currentSize && array[ child + 1 ].value < array[ child ].value ) 
				child++;  // child is the minimum of the children
			if( array[ child ].value < tmp )
			{
				int num = array[ k ].value;
				array[ k ].value = array[ child ].value;
				array[child ].value = num;
				string temp = array[ k ].label;
				array[ k ].label = array[ child ].label;
				array[ child ].label = temp;
				int num2 = location[array[k].label];
				location[array[k].label]=location[array[ child ].label];
				location[array[ child].label] = num2;
			}
			else
				break;
		}
		if (x>=2)
		{
			int num = array[ 2 ].value;
			array[ 2 ].value = array[ 3].value;
			array[3 ].value = num;
			string temp = array[ 2 ].label;
			array[ 2].label = array[ 3 ].label;
			array[ 3 ].label = temp;
			int num2 = location[array[2].label];
			location[array[2].label]=location[array[ 3 ].label];
			location[array[ 3].label] = num2;
		}

	
	}
	else if(array[k].value-1>=0 && command=="decrement")
	{
		array[k].value--;

			for( ; k > 1 && array[k].value <= array[ k / 2 ].value; k/= 2 )//logB
			{
				int num = array[ k ].value;
				array[ k ].value = array[ k / 2 ].value;
				array[ k / 2 ].value = num;
				string temp = array[ k ].label;
				array[ k ].label = array[ k / 2 ].label;
				array[ k / 2 ].label = temp;
				int num2 = location[array[k].label];
				location[array[k].label]=location[array[ k / 2 ].label];
				location[array[ k / 2 ].label] = num2;
			}

	
		}

}
BinaryHeap::BinaryHeap(int c )
{
	capacity=c+1;
	currentSize=0;
	array=new item[c+1];
}
BinaryHeap::~BinaryHeap()
{
	if(!isEmpty())
		delete [] array;
}

bool BinaryHeap::isEmpty( ) const
{
	if(currentSize==0)
		return true;
	else 
		return false;


}
void BinaryHeap::insert(  int x, string l)
{

    // Percolate up
    // Note that instead of swapping we move the hole up
    int hole = ++currentSize;

	for( ; hole > 1 && x < array[ hole / 2 ].value; hole /= 2 )
	{
        array[ hole ].value = array[ hole / 2 ].value;
		array[ hole ].label = array[ hole / 2 ].label;
		location[array[hole].label]=hole;
	}
    array[ hole ].value = x;
	array[hole].label=l;
	location[l]=hole;
}

void BinaryHeap::deleteMin( )//logB+C
{
    if( isEmpty( ) )
        return;
	int i=currentSize;
 	while( i>1 )//C
	{
		string what=array[i].label;
		int whaa=location[array[i].label];
		string what2=array[i-1].label;
		int whaa2=	location[array[i-1].label];
		location[array[i].label]=whaa2;
		i--;
	}
	location.erase(array[1].label);
    // move the last element to the first and shrink the array
	for(int l=1; l<currentSize;l++)//C
	{
		array[ l ].value = array[l+1].value;
		array[ l ].label = array[ l+1 ].label;
	}


	currentSize--;
    percolateDown( 1 ); //logB
}
const item  BinaryHeap::findMin( ) const
{
	if(currentSize==0)
		return 0;
	else
		return array[1]; //since its max heap, max element is the first one automatically 

}

void BinaryHeap::percolateDown( int hole ) //logB
{
	int child;
	int tmp = array[ hole ].value; // tmp is the item that will  be percolated down
    string tmplabel = array[ hole ].label;


	for( ; hole * 2 <= currentSize; hole = child )
	{
		child = hole * 2;
		if( child != currentSize && array[ child + 1 ].value < array[ child ].value )
			child++;  // child is the minimum of the children
		if( array[ child ].value < tmp )
		{
			array[ hole ].value = array[ child ].value;
			array[ hole ].label = array[ child ].label;
			location[array[hole].label] = hole;// swap hole and min child
		}
		else
			break;
	}
	array[ hole ].value = tmp;
	array[hole].label = tmplabel;
	location[array[hole].label] = hole;// place tmp in its final position
}



