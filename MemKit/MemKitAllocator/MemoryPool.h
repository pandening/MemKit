/**
 *      copyright C hujian 2016 version 1.0.0
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MEMKIT_MEMORYPOOL_H
#define MEMKIT_MEMORYPOOL_H

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include <cstring>

#define __HJSTL_PRIVATE_  private
#define __HJSTL_PUBLIC_   public
#define __HJSTL_BAD_ALLOC_ std::cout<<"out of memory"<<std::endl; exit(1)
#define __HJSTL_MIN_ALLOC_ 4 //the min align size is 4 bytes
#define __HJSTL_MAX_ALLOC_ 1024 //the max align size is 1024 bytes<1Mb>
#define __HJSTL_NFREELISTS_ (__HJSTL_MAX_ALLOC_/__HJSTL_MIN_ALLOC_)
#define __HJSTL_DEFAULT_ALLOC_NODES    20 //this is the default alloc nodes once .you can change this


//the free list node structure
//the user_data is funny,the memory will boot by self one by one.
//think of it yourself carefully.it's very funny and i laugh at myself.
union hjstl_free_list_node{
    union hjstl_free_list_node *free_list_link;
    char   user_data[1];
};

//this is the free_list array,tell compile not opt this array
hjstl_free_list_node* volatile hjstl_free_list[__HJSTL_NFREELISTS_];
//and the memory pool's size
char* hjstl_memory_pool_start = 0;
char* hjstl_memory_pool_end = 0;
size_t  hjstl_memory_pool_heap_size = 0;


//*this is the first level allocate.and the allocate handler
//the memory>...bytes,and others job handle by second level allocate
//the real-memory-pool is the second level allocate.
template<int inst>
class _hjstl_first_level_memory_pool_allocate{
__HJSTL_PRIVATE_://you should not touch this part code.<hujian>
    static void* hjstl_oom_malloc(size_t);
    static void* hjstl_oom_realloc(void*, size_t);

    //you also can set an new-handler to solve the out-of-memory
    //i will give the API to set/check the handler
    static void(*__hjstl_malloc_oom_handler)();

__HJSTL_PUBLIC_:
    /**
     * allocator
     * @param size
     * @return
     */
    static void * hjstl_allocate(size_t size) {
        void *result = malloc(size);
        if (0 == result) result = hjstl_oom_malloc(size);
        return result;
    }
    /**
     * de-allocator
     * @param mem
     */
    static void hjstl_deallocate(void* mem, size_t)
    {
        free(mem);
    }

    /**
     * re-alloc
     * @param oldmem
     * @param oldsz
     * @param newsz
     * @return
     */
    static void* hjstl_reallocate(void* oldmem, size_t oldsz, size_t newsz)
    {
        void* result = realloc(oldmem, newsz);
        if (0 == result) result = hjstl_oom_realloc(oldmem, newsz);
        return result;
    }
};

//implement the function of first level.
template<int inst>
void* _hjstl_first_level_memory_pool_allocate<inst>::hjstl_oom_malloc(size_t size)
{
    void(*my_malloc_oom_handler)();
    void* result;
    for (;;){
        my_malloc_oom_handler = __hjstl_malloc_oom_handler;
        if (0 == my_malloc_oom_handler) { __HJSTL_BAD_ALLOC_; }
        //run your oom-handler
        (*my_malloc_oom_handler)();
        result = malloc(size);  //re-try
        if (result) return result;
    }
}

template<int inst>
void* _hjstl_first_level_memory_pool_allocate<inst>::hjstl_oom_realloc(void* oldmem, size_t newsz)
{
    void(*my_malloc_oom_handler)();
    void* result;
    for (;;){
        my_malloc_oom_handler = __hjstl_malloc_oom_handler;
        if (0 == my_malloc_oom_handler){ __HJSTL_BAD_ALLOC_; }
        //run your handler
        (*my_malloc_oom_handler)();
        result = realloc(oldmem, newsz);
        if (result) return result;
    }
}
/**
 * no oom-handler
 */
template<int inst>
void(*_hjstl_first_level_memory_pool_allocate<inst>::__hjstl_malloc_oom_handler)() = 0;

//you can use this in your test-project
typedef _hjstl_first_level_memory_pool_allocate<0> hjstl_first_malloc;



//this is the second level allocate of hjstl memory pool
template<int inst>
class _hjstl_second_level_memory_pool_allocate{
    //you can not touch this part's code,just for root
__HJSTL_PRIVATE_:
    //round up to align min-align times bytes
    static size_t HJSTL_ROUND_UP(size_t bytes){
        return (((bytes)+__HJSTL_MIN_ALLOC_ - 1) & ~(__HJSTL_MIN_ALLOC_ - 1));
    }

    //find the free list,return the index
    static size_t HJSTL_FREELISTS_INDEX(size_t bytes){
        return (((bytes)+__HJSTL_MIN_ALLOC_ - 1) / __HJSTL_MIN_ALLOC_ - 1);
    }

    //return an node of size sz,and the left nodes append to the free list
    //so,after call this function,the free list maybe change<update>
    static void* hjstl_refill(size_t sz);

    //Allocate a chunk from os,nnodes maybe reduce if the memory pool no enough mem
    static char* hjstl_mem_pool_chunk_alloc(size_t sz, int &Nnodes);

__HJSTL_PUBLIC_://you can use this api in your project

    //allocate memory,but this function just handle the memory less max-align.
    //if the memory bigger max-align,call first level's allocate to handle it.
    static void* hjstl_allocate(size_t sz)
    {
        hjstl_free_list_node* volatile * my_free_list;
        hjstl_free_list_node*  result;
        if (sz > (size_t)__HJSTL_MAX_ALLOC_){
            return (hjstl_first_malloc::hjstl_allocate(sz));
        }
        //get the aim-free list
        my_free_list = hjstl_free_list + HJSTL_FREELISTS_INDEX(sz);
        result = *my_free_list;
        if (NULL== result){//no,this size's free list is empty,ok,re-fill it<default-nodes>
            void* r = hjstl_refill(sz);
            return (r);
        }
        //else,this type's free list is not empty,so,give an node to user
        //and update the free list.
        *my_free_list = result->free_list_link;
        return (result);
    }

    //deallocate,i don't know how to real-know the kernel of free...
    static void hjstl_deallocate(void* mem, size_t sz)
    {
        hjstl_free_list_node* volatile* my_free_list;
        hjstl_free_list_node*  append_link_node = (hjstl_free_list_node*)mem;
        //dispatch it to first level deallocate if the memory size bigger [1024] bytes
        if (sz > (size_t)__HJSTL_MAX_ALLOC_){
            hjstl_first_malloc::hjstl_deallocate(mem, sz);
            return;
        }
        //else,release this mem,and give it to free list.
        //append it front of free list.
        my_free_list = hjstl_free_list + HJSTL_FREELISTS_INDEX(sz);
        append_link_node->free_list_link = *my_free_list;
        *my_free_list = append_link_node;
    }

    //you cant to re-allocate memory?use this function
    static void* hjstl_reallocate(void* oldmem, size_t oldsz, size_t newsz);
};
//implement the second level memory pool.

//this is the memory pool,and return an node for user,maybe
//update the free list but maybe not update.
template<int inst>
char* _hjstl_second_level_memory_pool_allocate<inst>
    ::hjstl_mem_pool_chunk_alloc(size_t sz, int& Nnodes)
{
    char* result;
    size_t  total_bytes_to_alloc = sz*Nnodes;
    //the left memory of memory pool
    size_t left_bytes_mempool = hjstl_memory_pool_end - hjstl_memory_pool_start;
    //case 1,if the memory's memory is enough,just allocate.change the memory pool
    if (left_bytes_mempool >= total_bytes_to_alloc){
        result = hjstl_memory_pool_start;
        hjstl_memory_pool_start += total_bytes_to_alloc;
        return (result);
    }
        //case 2,check whether the left bytes reah 1 nodes of this size,if true,return
        //the actual nodes we can give.
    else if (left_bytes_mempool >= sz){
        //calc how many nodes we can allocate
        Nnodes = left_bytes_mempool / sz;
        //change the memory pool
        total_bytes_to_alloc = Nnodes*sz;
        result = hjstl_memory_pool_start;
        hjstl_memory_pool_start += total_bytes_to_alloc;
        return (result);
    }
        //case 3,ok,we find the free list not memory<maybe>,so,call first level allocate
        //to malloc a big chunk from os,and continue to allocate.
    else{
        size_t bytes_from_os = 8 * total_bytes_to_alloc + HJSTL_ROUND_UP(hjstl_memory_pool_heap_size >> 4);
        //whether old pool left some fragments?
        if (left_bytes_mempool > 0){
            hjstl_free_list_node* volatile* my_free_list =
                    hjstl_free_list + HJSTL_FREELISTS_INDEX(left_bytes_mempool);
            ((hjstl_free_list_node*)hjstl_memory_pool_start)->free_list_link =
                    *my_free_list;
            *my_free_list = (hjstl_free_list_node*)hjstl_memory_pool_start;
        }
        //reboot the memory pool
        hjstl_memory_pool_start = (char*)hjstl_first_malloc::hjstl_allocate(bytes_from_os);
        //if the first level memory ask error<oom?>
        if (0 == hjstl_memory_pool_start){
            //this is the last way to allocate.check others free list,and
            //release some others nodes,and re-try...
            hjstl_free_list_node* volatile* my_free_list, *find_pointer;
            for (int i = sz; i <= __HJSTL_MAX_ALLOC_; i += __HJSTL_MIN_ALLOC_){
                my_free_list = hjstl_free_list + HJSTL_FREELISTS_INDEX(i);
                find_pointer = *my_free_list;
                if (0 != find_pointer){
                    *my_free_list = find_pointer->free_list_link;
                    hjstl_memory_pool_start = (char*)find_pointer;
                    hjstl_memory_pool_end += i;
                    //retry till get enough memory,the nnodes will reduce after run sometimes
                    return (hjstl_mem_pool_chunk_alloc(sz, Nnodes));
                }
            }
        }//end of oom
        //adjust the memory pool,and re-try
        hjstl_memory_pool_heap_size += bytes_from_os;
        hjstl_memory_pool_end = hjstl_memory_pool_start + bytes_from_os;
        return (hjstl_mem_pool_chunk_alloc(sz, Nnodes));
    }
}


//refill the free list.maybe default-nodes,maybe none
template<int inst>
void* _hjstl_second_level_memory_pool_allocate<inst>::hjstl_refill(size_t sz)
{
    int default_nodes = __HJSTL_DEFAULT_ALLOC_NODES; //you can change the num
    char* alloc_chunk = hjstl_mem_pool_chunk_alloc(sz, default_nodes);

    hjstl_free_list_node* volatile * my_free_list;
    hjstl_free_list_node* result, *current_p, *next_p;
    //if the memory just return 1 nodes,ok,return it and kill self..
    if (1 == default_nodes)  return (alloc_chunk);
    //else,update the free list.
    my_free_list = hjstl_free_list + HJSTL_FREELISTS_INDEX(sz);
    //anyway,we need to give user one nodes.
    result = (hjstl_free_list_node*)alloc_chunk;
    //|*connect the nodes *|
    *my_free_list = next_p = (hjstl_free_list_node*)(alloc_chunk + sz);
    for (int i = 1;; i++){
        current_p = next_p;
        next_p = (hjstl_free_list_node*)((char*)next_p + sz);
        if (default_nodes - 1 == i){//this is the last node
            current_p->free_list_link = 0;
            break;
        }
        else{//this is not the last node,pointer to next
            current_p->free_list_link = next_p;
        }
    }
    //everything done,return the result.
    return (result);
}

//re-allocate
template<int inst>
void* _hjstl_second_level_memory_pool_allocate<inst>
        ::hjstl_reallocate(void* oldmem, size_t oldsz, size_t newsz)
{
    void* result;
    size_t  copy_size;
    if (oldsz>(size_t)__HJSTL_MAX_ALLOC_&&newsz >(size_t)__HJSTL_MAX_ALLOC_){
        return (hjstl_first_malloc::hjstl_reallocate(oldmem, oldsz, newsz));
    }
    //if same,just return
    if (HJSTL_ROUND_UP(newsz) == HJSTL_ROUND_UP(oldsz)) return (oldmem);
    //else,we use the second level allocate to do this job
    result = hjstl_allocate(newsz);
    //you should know if the newsz<oldsz,the data will lose.
    copy_size = newsz > oldsz ? oldsz : newsz;
    memcpy(result, oldmem, copy_size);
    //release the old memory
    hjstl_deallocate(oldmem, oldsz);
    return (result);
}
//you can use this in your project
typedef _hjstl_second_level_memory_pool_allocate<0> hjstl_second_malloc;
#endif //MEMKIT_MEMORYPOOL_H
