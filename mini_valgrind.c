/**
 * Mini Valgrind Lab
 * CS 241 - Fall 2016
 */

#include "mini_valgrind.h"
#include "print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef malloc
#undef realloc
#undef free



/*
 * Replace normal malloc, this malloc will also create meta data info
 * and insert it to the head of the list.
 * You have to malloc enough size to hold both the size of your allocated
 * and the meta_data structure.
 * In this function, you should only call malloc only once.
 *
 * @param size
 *	Size of the memory block, in bytes.
 * @param file
 *	Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *	Line in the file that causes memory leak
 *
 * @return
 *	On success, return a pointer to the memory block allocated by
 *	the function. Note that this pointer should return the actual
 *	address the pointer starts and not the meta_data.
 *
 *	If the function fails to allocate the requested block of memory,
 *	return a null pointer.
 */
void *mini_malloc(size_t size, const char *file, size_t line) {
    
    meta_data* md = (meta_data*)malloc(sizeof(meta_data) + size);
    if (!md) return NULL;
    
    insert_meta_data(md, size, file, line);
    return (void*)(md + 1);
}

/*
 * Replace normal realloc, this realloc will also first check whether the
 * pointer that passed in has memory. If it has memory then resize the memory
 * to it. Or if the pointer doesn't have any memory, then call malloc to
 * provide memory.
 * For total usage calculation, if the new size is larger than the old size,
 * the total usage should increase the difference between the old size and the
 * new size. If the new size is smeller or equal to the old size, the total
 * usage should remain the same.
 * You have to realloc enough size to hold both the size of your allocated
 * and the meta_data structure.
 * In this function, you should only call malloc only once.
 *
 * @param ptr
 *      The pointer require realloc
 * @param size
 *	Size of the memory block, in bytes.
 * @param file
 *	Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *	Line in the file that causes memory leak
 *
 * @return
 *	On success, return a pointer to the memory block allocated by
 *	the function. Note that this pointer should return the actual
 *	address the pointer starts and not the meta_data.
 *
 *	If the function fails to allocate the requested block of memory,
 *	return a null pointer.
 */
void *mini_realloc(void *ptr, size_t size, const char *file, size_t line) {
  // your code here
  return NULL;
}

/*
 * Replace normal free, this free will also delete the node in the list.
 *
 * @param ptr
 *	Pointer to a memory block previously allocated. If a null pointer is
 *	passed, no action occurs.
 */
void mini_free(void *ptr) {
    
    //freeing NULL just well, i guess, i mean , like just, uhmmm, ahhh DOES NOTHING!
    if(!ptr) return;
    
    meta_data* md = (meta_data*)ptr;
    md-=1;
    remove_meta_data((void*)md);
  
}

/*
 * Helper function to insert the malloc ptr node to the list.
 * Accumulate total_usage here.
 *
 * @param md
 * 	Pointer to the meta_data
 * @param size
 *	Size of the memory block, in bytes.
 * @param file
 *	Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *	Line in the file that causes memory leak
 */
void insert_meta_data(meta_data *md, size_t size, const char *file,
                      size_t line) {
  
    md->size = size;
    md->line_num = line;
    strncpy(md->file_name, file, MAX_FILENAME_LENGTH);
    md->next = NULL;
    
    /** insert this metadata into the linked list of allocations **/
    
    
    //empty to begin with
    if (!head)
        head = md;
    //head already exists. Make this one point to the current head and reset head to this one.
    // CONSTANT TIME BRUHHH!!!
    else{
        md->next = head;
        head = md;
    }
    
    total_usage+=size;
    
}

/*
 * Helper function to remove the free ptr node from the list.
 * Add to total_free here.
 *
 * @param ptr
 *	Pointer to a memory block previously allocated.
 */
void remove_meta_data(void *ptr) {
    
    //freeing memory without allocating any first? WHATTTTT? NOOOOOOOOOOO!!!!
    if (!head){
        bad_frees++;
        return;
    }
    
    meta_data* found = head;
    meta_data* before = head;
    int begin_before = 0;
    while (found != NULL) {
        if ( found == (meta_data*)ptr )
            break;
        found = found->next;
        if (begin_before) before = before->next;
        begin_before = 1;
    }
    
    //Trying to free something that was never allocated
    if (!found)
        bad_frees++;
    
    //we found a match
    else{
      
        //if meta_data struct is at head
        if (head == found) {
            head = found->next;
        }
        //either middle or back
        else{
            
            before->next = found->next;
        }
        
        
        total_free+=found->size;
        free(found);
    }
    
    
    
}

/*
 * Deletes all nodes from the list. Called when the program exits so make sure
 * to not count these blocks as freed.
 */
void destroy() {
  
    meta_data* md = head;
    
    while (md != NULL) {
        
        meta_data* temp = md;
        md = md->next;
        free(temp);
    }
    
    
}

/*
 * Print mini_valgrind leak report.
 */
void print_report() {
    print_leak_info(head, total_usage, total_free, bad_frees);
}
