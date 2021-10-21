#include <iostream>
#include "my_heap.h"
using namespace std;

My_heap::My_heap()
{
	heap_begin= NULL;
	blk = NULL;
	used_bytes=0;
}// creates an empty my_heap

My_heap::~My_heap() //destructor
{
	int leak=0;
	memory_block*temp=heap_begin;
	while(temp!=NULL)
	{
		if(temp->used==true)
			leak+=temp->size;	//for keeping the used size which is leak
		memory_block*temp2=temp;
		temp=temp->right;
		delete temp2;
	}
	cout <<"At destruction, the heap had a memory leak of " << leak <<" bytes." <<endl;

}

memory_block* My_heap::bump_allocate(int num_bytes) //function to allocate new memory block at the end
{
	if (heap_begin==NULL && num_bytes<= MAX_CAPACITY) //if heap is empty, make sure that capacity isnt exceeded
	{				
		memory_block *temp= new memory_block();
		temp->size=num_bytes;
		temp->used=true;
		temp->starting_address=0;
		blk=temp;//we need to initialize last and first block pointers
		heap_begin=temp; 
		used_bytes=num_bytes;

		return temp;
			
	}
	else if(used_bytes+num_bytes <=MAX_CAPACITY) //if we are adding to nonempty heap, make sure that capacity isnt exceeded
	{
		memory_block *temp= new memory_block();
		temp->size=num_bytes;
		temp->used=true;
		blk->right=temp; //new block should be at the right of the previous last node
		temp->left=blk; //new last block is newly added one 
		temp->starting_address=blk->starting_address+blk->size;
		blk=temp;
		used_bytes+=num_bytes;
		return temp;
	}
	else
		return NULL;
}

memory_block* My_heap::first_fit_allocate(int num_bytes) //use the first free block
{
	memory_block* temp = heap_begin;
	bool found=false;
	if (used_bytes+num_bytes<MAX_CAPACITY) //make sure that capacity isnt exceeded
	{
		while( found==false && temp!=NULL) //found is to check if there is a block: non used and having appropriate size
		{
			if(temp->used==false && temp->size>=num_bytes)
				found=true; //if an appropriate block is found
			else
				temp=temp->right; //if not, keep iterating
				
		}
		if(temp==NULL) //if no such block is found
		{
			return bump_allocate(num_bytes); //add to the very end
					
		}
		else //if found, temp is pointing to that block now
		{
			temp->used=true;
			return temp;
	
		}	
	}
	else //if it exceeds the capacity do nothing
		return NULL;
}

memory_block* My_heap::best_fit_allocate(int num_bytes) //use the block that will lead to the smallest amount of wasted memory
{
	memory_block*checker, *runner; //we need to pointer, one to run one to check
	bool found=false;
	runner=heap_begin; //runner starts at the beginning
	if(heap_begin==NULL) //if heap is empty
	{
		return bump_allocate(num_bytes); //create new block
	}
	else
	{
		while(runner!=NULL && found==false) //as long as runner dont find suitable block
		{
			if(runner->used==false && runner->size>=num_bytes) //if we found and nonused block with enough size
				found=true;
			else
				runner=runner->right; //keep iterating if not found
		}
		if (runner==NULL) //if there isnt any nonused block 
		{
			return bump_allocate(num_bytes);	//add to the end of the heap
		}
	}
	checker=runner; //if runner is not null meaning it is pointing a non used block with enough size, checker goes there also
	while(runner!=NULL)//runner is to go over all heap blocks
	{
		if( runner->used!=true&& runner->size >=num_bytes&& (runner->size-num_bytes  ) < (checker->size-num_bytes)) //if runner finds a non used block with smaller amount of wated memory than checker
		{
			checker=runner; //checker comes to runners place 
		}
		runner=runner->right; //runner keep iterating
	}
	checker->used=true; //at the end, checker ends up with best block hence we make it used true
	return checker;
}

memory_block* My_heap::first_fit_split_allocate(int num_bytes) //use the first free block and make another block using excess size
{
			
	memory_block* temp = heap_begin; 
	bool found=false;
	if (used_bytes+num_bytes<MAX_CAPACITY) //if the desired fitting doesnt exceed the size 
	{
		while( found==false && temp!=NULL) 
		{
			if(temp->used==false && temp->size>=num_bytes) //when an unused block with enough size is found
				found=true;
			else
				temp=temp->right;
				
		}
		if(temp==NULL) //if there is no such block or no block at all
		{
			return bump_allocate(num_bytes); //add a new block
					
		}
		else //we have a pointer pointing at the appropriate block
		{
			int newsize=(temp->size)-num_bytes;
			if(newsize==0) //is size of block is equal to num bytes we want to allocate
			{
				temp->used=true; //just allocate the whole block
				return temp;
			}
			else { //if we have excess size in that block
				temp->used=true; 
				temp->size=num_bytes; //size of block should be axactly the num bytes
				memory_block* second= new memory_block; //we create another block
				second->size=newsize; //size is the excess size
				second->starting_address=temp->starting_address+num_bytes; 
				second->right=temp->right;
				second->left=temp;
				temp->right->left=second;
				temp->right=second; //insertion of second block
				deallocate(second); //deallocation must be done and merging will be done accordingly in this function
				return temp; 
			}
	
		}	
	}
	else //if num bytes exceeds the capacity 
		return NULL;
}

void My_heap::deallocate(memory_block* block_address){
 	if(heap_begin!=NULL)
	{
		block_address->used=false;

		if(heap_begin==blk && heap_begin==block_address)
		{//do nothing
		}

		else if(block_address->left==NULL && block_address->right->used==true ) //if the first block is to be deallocated and right block is not deallocated 
		{
			//dont do anything
		}
		else if(block_address->right==NULL && block_address->left->used==true) //if the last block is to be deallocated and left one is not deallocated 
		{
			//dont do anything
		}
		else if(block_address->right==NULL && block_address->left->used==false) //if the last block is to be deallocated and left one is already deallocated
		{
			block_address->left->size+=block_address->size;
			blk=block_address->left;
			block_address->left->right=NULL;
			delete block_address;
		}
		else if(block_address->left==NULL && block_address->left->used==false) //if the first block is to be deallocated and right block is already deallocated
		{
			block_address->size+=block_address->right->size;
			if (block_address->right->right!=NULL){ //if there are not only two blocks(head and right neighbor)
				memory_block*temp=block_address->right;
				block_address->right->right->left=block_address;
				block_address->right=block_address->right->right;
						
				delete temp;
			}
			else //if there are only two blocks(head and right neighbor)
			{
				memory_block*temp=block_address->right;
				block_address->right=block_address->right->right;
				blk=heap_begin; //because now we have only one block
				delete temp;
			}
		}
		else if(block_address->left!=NULL && block_address->right!=NULL) //if a middle block is to be deallocated
		{
			if(block_address->left->used==false && block_address->right->used==true) //if left neighbor is already deallocated
			{
				block_address->left->size+=block_address->size;
				block_address->left->right=block_address->right;
				block_address->right->left=block_address->left;
				delete block_address;			
			}
			else if(block_address->right->used==false && block_address->left->used==true) //if right neighbor is already deallocated
			{
				block_address->size+=block_address->right->size;
				if(block_address->right->right==NULL)//if right neighbor is the last block
				{
					memory_block * temp = block_address->right;
					block_address->right=NULL;
					blk=block_address;//because we delete the old tail
					delete temp;				
				}
				else
				{
					memory_block * temp = block_address->right;
					block_address->right=block_address->right->right;
					temp->right->left=block_address;
					delete temp;
				}
				
			}
			else if (block_address->right->used==false&&block_address->left->used==false) //if both right and left neighbor are already deallocated
			{
 				block_address->left->size+=block_address->size + block_address->right->size;
				if(block_address->right->right==NULL)//if right neighbor is the last block
				{
					memory_block * temp = block_address->right;
					memory_block * temp2 = block_address;
					block_address->left->right=NULL;
					blk=block_address; //because we delete last two blocks
					delete temp, temp2;				
				}
				else
				{
					memory_block * temp = block_address->right;
					memory_block * temp2 = block_address;
					block_address->left->right=temp->right;
					temp->right->left=block_address->left;
			
					delete temp, temp2;	
				}
					
					
			}
				
				
		}
	}

}


void My_heap::print_heap()
{
	int used_memory=0, memory_block_num=0, num_of_used_blocks=0, free_memory=0;
	memory_block*temp=heap_begin;

	while(temp!=NULL)
	{
		if(temp->used==false)
		{
			free_memory+=temp->size; //used=false ones'sizes are free memory

		}
		else if(temp->used==true){
			used_memory+=temp->size; //used=true ones'size are used memory
		num_of_used_blocks+=1;
		}
		memory_block_num+=1;
		temp=temp->right;
	}
	cout << "Maximum capacity of heap: " <<MAX_CAPACITY<< "B" <<endl;
	cout << "Currently used memory (B): " << used_memory <<  endl;
	cout << "Total memory blocks: "<< memory_block_num << endl;
	cout << "Total used memory blocks: " << num_of_used_blocks << endl;
	cout << "Total free memory blocks: " << memory_block_num-num_of_used_blocks << endl;
	cout <<"Fragmentation: " << get_fragmantation() << "%" << endl;
	cout << "------------------------------"<<endl;
	int index=0;
	memory_block*iteratorx=heap_begin;
	while(iteratorx!=NULL)
	{
		if(iteratorx->used ==true)
		cout << "Block " <<index<<"\t\tUsed: " << "True" <<"\tSize (B): " << iteratorx->size << "\tStarting Adress: 0x" << hex << (iteratorx->starting_address) << dec << "\n";
		else
			cout << "Block " <<index<<"\t\tUsed: " << "False" <<"\tSize (B): " << iteratorx->size << "\tStarting Adress: 0x" << hex << (iteratorx->starting_address) << dec << "\n";

		index++;
		iteratorx=iteratorx->right;
			
	}
	cout << "------------------------------"<<endl;
	cout << "------------------------------"<<endl;


}

float My_heap::get_fragmantation()
{
	memory_block*temp=heap_begin;
	if(heap_begin==NULL)
		return 0; //empty heap has 0 fragmentation
	float allocated_memory=0.0, nonallocated_memory=0.0, biggest_size=0.0, freememory;
	float fragment;
	while(temp!=NULL)
	{
		if(temp->used==false)
		{
			nonallocated_memory+=temp->size;
			if(temp->size >biggest_size)
				biggest_size=temp->size; //to keep the biggest size among unused blocks

		}
		if (temp->used==true)
			allocated_memory+=temp->size;
		temp=temp->right;
	}
	freememory=MAX_CAPACITY-allocated_memory; 
	int total_memory=allocated_memory+ nonallocated_memory;
	if((MAX_CAPACITY-total_memory)>biggest_size) //if the remaining memory is bigger than the biggest size of unused block
		biggest_size=MAX_CAPACITY-total_memory; //we take it as biggest size
	if (freememory==0) //if heap uses all capacity
		return 0; //because we cant do division by 0 in the fragment equation
	fragment=((freememory-biggest_size)/freememory)*100;
	return fragment;
		
}
