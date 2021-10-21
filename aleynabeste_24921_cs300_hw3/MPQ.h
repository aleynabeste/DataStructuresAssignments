#include <iostream>
#include <string>
#include <vector>

using namespace std;
template <class Comparable> 
struct item //I am gonna use this in the heap
{
	int label; //id
	Comparable value; 
	item(Comparable val=0, int l=0)
	{
		value=val; 
		label=l;
	};
 //Constructor for struct
};

template <class Comparable>
class MPQ // maxheap
{
public:
	MPQ(int c); 
	~MPQ();
	void insert(  Comparable & x, int label ); 
	Comparable remove( int label ); 
	Comparable GetMax( ) const; 
	bool isEmpty( ) const; 
	//void makeEmpty();
	int getCurrentSize(); 
	void percolateDown( int hole); 

private:
	int currentSize;  //num of elements in heap
	int volume; //its the capacity
	item<Comparable>* heap;   //heap array
	int * locationArray;  //location array
	
};
template <class Comparable>
MPQ<Comparable>::MPQ(int c) 
{
	currentSize = 0; // Because we didtn put anything yet
	heap= new item<Comparable>[c];
	locationArray=new int[c];
	volume=c;
}
template <class Comparable>
MPQ<Comparable>::~MPQ() // destructor.
{ 
	if(!isEmpty())
	{
		delete []heap; //since they are dynamically allocated we need to delete them
		delete []locationArray;
		
	}

}

template <class Comparable>
int MPQ<Comparable>::getCurrentSize()//accessor
{ 
	return currentSize;
}

template <class Comparable>
bool MPQ<Comparable>::isEmpty() const
{ 
	if (currentSize==0) //if there isnt anything yet
		return true;
	else
		return false;
}
template <class Comparable>
void MPQ<Comparable>::insert(  Comparable & x , int label_input)//this function is taken from lecture slides, with some changes
{
  
    int hole = ++currentSize;
  // Percolate up
    // Note that instead of swapping we move the hole up
    for( ; hole > 1 && x > heap[ hole / 2 ].value; hole /= 2 )
	{
        heap[ hole ].value = heap[ hole / 2 ].value;
	    heap[ hole ].label = heap[ hole / 2 ].label;
		locationArray[heap[hole].label]=hole;
	}
	heap[hole].value = x;
	heap[hole].label = label_input;
	
	locationArray[label_input] = hole;
 
}

template <class Comparable>
Comparable MPQ<Comparable>::GetMax() const //this function returns the max value in array if not empty, otherwise returns 0
{
	if(currentSize==0)
		return 0;
	else
		return heap[1].value; //since its max heap, max element is the first one automatically 
}

template<class Comparable>
Comparable MPQ<Comparable>::remove( int label )//this function removes an element from the vector
{
	if(isEmpty())
		return 0;
	int loc_on_heap = locationArray[label];
    if (loc_on_heap <0)
       return 0;

	heap[locationArray[label]].value =  heap[currentSize].value;
    heap[locationArray[label]].label =  heap[currentSize].label;
	
	 //heap[currentSize].label=0;
	 //heap[currentSize].value=0;
    currentSize-- ;
	int i=locationArray[label];
	percolateDown(i);  
	return locationArray[label];
}

template<class Comparable>
void MPQ<Comparable>::percolateDown( int hole) //this function is taken from lecture slides, with some changes
{
	       
	int child;
	Comparable tmp = heap[ hole ].value; // tmp is the item that will  be percolated down
    int tmplabel = heap[ hole ].label;


	for( ; hole * 2 <= currentSize; hole = child )
	{
		child = hole * 2;
		if( child != currentSize && heap[ child + 1 ].value > heap[ child ].value )
			child++;  // child is the minimum of the children
		if( heap[ child ].value > tmp )
		{
			heap[ hole ].value = heap[ child ].value;
			heap[ hole ].label = heap[ child ].label;
			locationArray[heap[hole].label] = hole;// swap hole and min child
		}
		else
			break;
	}
	heap[ hole ].value = tmp;
	heap[hole].label = tmplabel;
	locationArray[heap[hole].label] = hole;// place tmp in its final position
  
}


