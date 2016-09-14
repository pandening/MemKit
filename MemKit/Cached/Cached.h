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

#ifndef MEMKIT_CACHED_H
#define MEMKIT_CACHED_H

#include "../MemKitAllocator/SimpleMemAlloc.h"
#include "SimpleConstructor.h"
#include "MemSnapshot.h"

#define  debug 0

typedef void* pointer;
/**
 * the cache data
 */
typedef struct cache_data{
    pointer next;
    std::string data;
}cache_data_t;

/**
 * cache.
 */
class Cached {
private:
    typedef cache_data_t* link_type;
    link_type this_node;
    /**
     * the alloc
     */
    typedef SimpleMemAlloc<cache_data_t> Alloc;
    /**
     * the mem
     */
    MemKit* Mem;
    /**
     * the timer handler
     */
    Mem_Snapshot* SnapShot;
    int Size;
    int Capacity;
protected:
    /**
     * get an node from mempory
     * @return
     */
    link_type get_node(){
        return Alloc::allocate();
    }
    /**
     * create an node
     * @param store_id
     * @param key
     * @param value
     */
    void create_node(link_type& node,const String store_id,const String key,const String value){
        String ikv=store_id;ikv+=",";ikv+=key;ikv+=",";ikv+=value;
        constructor(&node->data,ikv);
    }
    /**
     * back to mem pool
     * @param node
     */
    void put_node(link_type node){
        Alloc::deallocate(node);
    }
    /**
     * destroy an node
     * @param node
     */
    void destroy_node(link_type node){
        destroy(&node->data);
        put_node(node);
    }
    /**
     * clear
     * @param capacity
     */
    void clear(bool flag=true){
        this->Size=0;
        if(!flag){
            this->Capacity=0;
        }
        link_type t=this->this_node;
        while(t->next!=NULL){
            link_type  del=t;
            t= (link_type) t->next;
            put_node(del);
        }
        /**
         * this is the header pointer,do not delete/free it~
         */
        this->this_node->next=NULL;
    }

public:
    /**
     * you should offer a capacity
     * @param capacity
     * @return
     */
    Cached(int capacity=100){
        this->Size=0;
        this->Capacity=capacity;
        this->Mem=MemKit::getInstance(1024);
        this->SnapShot=new Mem_Snapshot(0,10,-1,this->Capacity-1);
    }
    /**
     * put an node into cache
     * @param id
     * @param key
     * @param value
     * @return
     */
    bool put(String id,String key,String value){
        if(this->size()==this->capacity()){
            return false;
        }
        /**
         * create the header pointer
         */
        if(this->this_node==NULL){
            this->this_node=get_node();
            create_node(this_node,id,key,value);
            this->this_node->next=NULL;
            this->Size++;
        }else if(!exist(id,key)){
            link_type np=this->get_node();
            create_node(np,id,key,value);
            np->next=this->this_node;
            this->this_node=np;
            this->Size++;
            return true;
        }
    }
    /**
     * get the value
     * @param id
     * @param key
     * @return
     */
    String get(String id,String key){
        link_type query=this->exist(id,key);
        if(NULL==query){
            return NULL;
        }else{
            return MemKitUtils::split(query->data,",")[2];
        }
    }

    /**
     * check
     * @param id
     * @param key
     * @return
     */
    link_type exist(String id,String key){
        if(this->this_node==NULL||this->size()==0){
            return NULL;
        }
        link_type traversal=this->this_node;
        while(traversal!=NULL){
            os<<"=>"<<traversal->data<<el;
            if(MemKitUtils::split(traversal->data,",")[0]==id&&
                    MemKitUtils::split(traversal->data,",")[1]==key){
                return traversal;
            }
            traversal= (link_type) traversal->next;
        }
        return NULL;
    }
    int size(){
        return this->Size;
    }

    int capacity(){
        return this->Capacity;
    }
    /**
     * re-set the capacity
     * @param sz
     */
    void setCapacity(int sz){
        if(sz==0){
            this->clear(sz);
        }
        if(sz>=this->capacity()){
            this->Capacity=sz;
        }else{
            if(this->size()<=sz){
                this->Capacity=sz;
            }else{
                link_type traversal=this->this_node;
                int dis=this->size()-sz;
                while((dis--)!=0){
                    link_type del= (link_type) traversal->next;
                    traversal->next=del->next;
                    put_node(del);
                    traversal= (link_type) traversal->next;
                }
                this->Capacity=sz;
                this->Size=sz;
            }
        }
    }
    /**
     * ok,this function will create an simple mem snapshot for user.
     * but this is very slowly.
     * @return
     */
    bool create_mem_snapshot(){
        if(this->Mem->size()==0){
            return false;
        }
        std::vector<snapshot_t> vec=this->SnapShot->getSnapShot();
        std::vector<snapshot_t>::iterator vit=vec.begin();
        if(vec.size()!=0){
            for(;vit!=vec.end();vit++){
                this->put((*vit).id,(*vit).key,(*vit).value);
            }
        }
        return true;
    }
};

#endif //MEMKIT_CACHED_H
