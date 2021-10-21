#include <string>
#include <iostream>

using namespace std;
#define MAX_CAPACITY 512 // the size capacity of Img_heap


struct memory_block
{
	memory_block* right; // node to the right
	memory_block* left; // node to the left
	bool used; // if this memory block is reserved or not
	int size; // the number of bytes reserved in Img_heap
	int starting_address; // the starting address in Img_heap
	// of the memory reserved by this block
	//The area that a memory block B is responsible for, is the area within the bytes [B->starting_address, B->starting_address+B->size)

	memory_block::memory_block (): left(NULL), right(NULL), used(true) {} //default constructor
	

};


class My_heap{
	private:
		memory_block* heap_begin; //pointer at the first memory block
		memory_block* blk; //pointer at the last memory block
		int used_bytes; // total number of allocated bytes in img heap
	public:
		My_heap();

		~My_heap() ;

		memory_block* bump_allocate(int num_bytes);

		memory_block* first_fit_allocate(int num_bytes);
		memory_block* best_fit_allocate(int num_bytes);

		memory_block* first_fit_split_allocate(int num_bytes);


		void deallocate(memory_block* block_address);
		void print_heap();
		float get_fragmantation();

};