#include "my_malloc.h"
#include <assert.h>
/* You *MUST* use this macro when calling my_sbrk to allocate the
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* Please use this value as your canary! */
#define CANARY 0x2110CAFE

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you may receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif


/* our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist;

static metadata_t * find_and_set_block(size_t);
static metadata_t * remove_from_list(metadata_t *, size_t);
static void add_to_freelist(metadata_t *);
void* my_malloc(size_t size)
{
	static int first_call = 1;
  	if (first_call || !freelist)
  	{
  		freelist = (metadata_t *) my_sbrk(SBRK_SIZE);
  		if (!freelist)
  		{
  			ERRNO = OUT_OF_MEMORY;
  			return NULL;
  		}
  		freelist->next = NULL;
  		freelist->prev = NULL;
  		freelist->block_size = SBRK_SIZE;
  		freelist->request_size = 0;
  		first_call = 0;

  	}
  	metadata_t * free_block = find_and_set_block(size);
  	if (!free_block)
  	{
  		return NULL;
  	}
  	void * ret = ((char *) free_block) + sizeof(metadata_t) + sizeof(int);
  	ERRNO = NO_ERROR;
  	return ret;
}
static metadata_t* find_and_set_block(size_t request_size)
{
	//TODO may need a loop to handle case where you have to keep
	//expanding the heap
	if (request_size + sizeof(metadata_t) + 2 * sizeof(int) > SBRK_SIZE)
	{
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}
	metadata_t * curr_block = freelist;
	metadata_t * smallest_possible_block = NULL;
	size_t needed_block_size = request_size
								+ (2 * sizeof(int))
								+ sizeof(metadata_t);
	short smallest_difference = SBRK_SIZE * 5;
	while (curr_block)
	{
		short difference = curr_block->block_size - needed_block_size;
		if (difference >= 0 && difference <= smallest_difference)
		{
			smallest_difference = difference;
			smallest_possible_block = curr_block;
		}
		curr_block = curr_block->next;
	}
	if (smallest_possible_block)
	{
		//split the block if possible
		curr_block = remove_from_list(smallest_possible_block, request_size);
		char * c_curr_block = (char *) curr_block;
		int * p_left_canary = (int *)(c_curr_block + sizeof(metadata_t));
		*p_left_canary = CANARY;
		int * p_right_canary = (int *) ((c_curr_block) + sizeof(metadata_t) + sizeof(int) + request_size);
		*p_right_canary = CANARY;			
		return curr_block;
	}
	else
	{
		//call my_sbrk to expand the heap and make sure result isn't null
		curr_block = freelist;
		while(curr_block->next)
		{
			curr_block = curr_block->next;
		}
		curr_block->next = my_sbrk(SBRK_SIZE);

		if (curr_block->next)
		{
			curr_block->next->prev = curr_block;
			curr_block->next->next = NULL;
			curr_block->next->request_size = 0;
			curr_block->next->block_size = SBRK_SIZE;
		}
		else
		{
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		curr_block = curr_block->next;
		char * c_curr_block = (char *) curr_block;
		int * p_left_canary = (int *)(c_curr_block + sizeof(metadata_t));
		*p_left_canary = CANARY;
		int * p_right_canary = (int *) ((c_curr_block) + sizeof(metadata_t) + sizeof(int) + request_size);
		*p_right_canary = CANARY;

		curr_block->request_size = request_size;
		curr_block->block_size = sizeof(metadata_t) + 2 * sizeof(int) + request_size;
		curr_block->prev->next = (metadata_t *)(c_curr_block
							+ request_size
							+ sizeof(metadata_t)
							+ 2 * sizeof(int));
		metadata_t * next_block = curr_block->prev->next;
		next_block->request_size = 0;
		next_block->next = NULL;
		next_block->block_size = SBRK_SIZE - curr_block->block_size;
		next_block->prev = curr_block->prev;
		//TODO what if the next block isn't big enough to hold data
		//will have to give the user the entire block
		return curr_block;
	}

}

static metadata_t *
remove_from_list(metadata_t * block, size_t request_size)
{
	metadata_t * ret = block;
	short min_block_size = 2 * sizeof(int) + sizeof(metadata_t) + 1; 
	short new_block_size = 2 * sizeof(int) + sizeof(metadata_t) + request_size;
	if (block->block_size - new_block_size >= min_block_size)
	{
		metadata_t * temp_next = block->next;
		block->next = (metadata_t*) ((char *)block
						+ 2 * sizeof(int)
						+ sizeof(metadata_t)
						+ request_size);
		if (temp_next)
		{
			temp_next->prev = block->next;
		}
		block->next->next = temp_next;
		block->next->prev = block->prev;
		if (block->prev)
		{
			block->prev->next = block->next;
		}
		else
		{
			freelist = block->next;
		}
		block->next->block_size = block->block_size - new_block_size;
		block->block_size = new_block_size;
		block->request_size = request_size;
	}
	else
	{
		if (block->prev)
		{
			block->prev->next = block->next;
		}
		else 
		{
			freelist = block->next;
		}
		if (block->next)
		{
			block->next->prev = block->prev;
		}
		block->request_size = request_size;
	}
	return ret;
}
void my_free(void* ptr)
{
	//todo what happens if ptr is null??
	if(ptr)
	{
		int * p_left_canary = (int*) (((char *)ptr) - sizeof(int));
		//make sure the left canary isn't corrupted
		//it should still have the appropriate value
		if (*p_left_canary != CANARY)
		{
			//todo set appropriate error
			ERRNO = CANARY_CORRUPTED;
			return;
		}
		metadata_t * metadata = (metadata_t *)
							(((char*) ptr)
								- sizeof (int)
								- sizeof (metadata_t));
		int * p_right_canary = (int *) (((char *)ptr) + metadata->request_size);
		if (*p_right_canary != CANARY)
		{
			ERRNO = CANARY_CORRUPTED;
			return;
		}
		if (freelist && freelist->next)
		{
			assert(freelist->next->prev == freelist);
		}
		add_to_freelist(metadata);
		if (freelist && freelist->next)
		{
			assert(freelist->next->prev == freelist);
		}
	}
}

static void add_to_freelist(metadata_t * block)
{
	metadata_t * curr = freelist;
	block->request_size = 0;
	if (!curr)
	{
		//the freelist is empty, so just add this block back
		freelist = block;
		freelist->next = NULL;
		freelist->prev = NULL;
		//setting to null may be unnecessary but better safe than sorry
	}
	else
	{
		//try to find its buddies if possible
		//otherwise add it to the end of the list
		metadata_t * left = NULL;
		metadata_t * right = NULL;
		while (curr)
		{
			if (((char*)curr) + curr->block_size == block)
			{
				//found its left buddy
				left = curr;
			}
			if (((char*)block) + block->block_size == curr)
			{
				//found its right buddy
				right = curr;
			}
			if (!curr->next && !left && !right) //if we reached the end of the list
			{
				curr->next = block;
				block->next = NULL; //just to be sure
				block->prev = curr;
				break;
			}
			curr = curr->next;
		}
		if (left && right)
		{
			if (freelist == right)
			{
				freelist = right->next;
			}
			
			left->block_size += block->block_size + right->block_size;

			if (right->next)
			{
				right->next->prev = right->prev;
			}

			if (right->prev)
			{
				right->prev->next = right->next;
			}
		}
		else if (left)
		{
			left->block_size += block->block_size;
		}
		else if (right)
		{
			if (right == freelist)
			{
				freelist = block;
			}
			if (right->next)
			{
				right->next->prev = block;
			}
			if (right->prev)
			{
				right->prev->next = block;
			}
			block->block_size += right->block_size;
			block->prev = right->prev;
			block->next = right->next;
		}
	}
}


