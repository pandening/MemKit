/**
 *      CopyRight C hujian 2016 version 1.0.0
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
#ifndef MEMKIT_MEMKIT_H
#define MEMKIT_MEMKIT_H

#define MEMKIT_PRIVATE private
#define MEMKIT_PUBLIC  public
#define MEMKIT_PROTECTED protected

#include<map> /*for map*/
#include<string> /*for string*/
#include <string.h>
#include <iostream> /*for cout*/
#include <time.h>
#include <fstream> /*for I/O*/

#include "../conf/Configure.h"

typedef std::string String;
typedef long Long;
/**
 * this is the only instance of the MemKit class.
 */
static class MemKit* memKit_instance;

/**
 * the ttl information
 */
typedef struct ttl{
    long TTL;
    long putTime;
}ttl_t;
/**
 * the ttl setter
 */
typedef struct TTL{
    String store_id;
    String key;
    ttl_t ttl_info;
}TTL_info;

/**
 * this is a simple memcache by c++.
 */
class MemKit {
    /**
     * THE PRIVATE PART
     */
MEMKIT_PRIVATE:
    /**
     * this is the storage,you should offer an store id to put/get
     */
    std::map<String,std::map<String,String>> storage;
    /**
     * time to live,one key-one time.
     */
    std::map<String,std::map<String,ttl_t>> time_to_live;
    /**
     * the capacity,you should set the capacity
     */
    Long storage_capacity;
    /**
     * the current size
     */
    Long storage_size;
    /**
     * the config
     */
    MemKitConfig* config;
    /**
     * total mem size
     */
    int total_mem_size;
    /**
     * used mem size
     */
    int used_mem_size;
    /**
     * free mem size
     */
    int free_mem_size;

    /**
     * THE PROTECTED PART
     */
MEMKIT_PROTECTED:
    /**
     * just get the current millis
     * @return
     */
    Long  getMillis(){
        return time((time_t*)NULL);
    }
    /**
     * set a ttl for a key
     * @param info
     */
    void setTTL(String store_id,String key,ttl_t info){
        std::map<String,std::map<String,ttl_t>>::iterator itr=this->time_to_live.find(store_id);
        if(itr==this->time_to_live.end()){//no this storage
            std::map<String,ttl_t> st;
            st.insert(std::pair<String,ttl_t>(key,info));
            this->time_to_live.insert(std::pair<String,std::map<String,ttl_t>>(store_id,st));
        }
        else{//exist this storage
            (*itr).second.insert(std::pair<String,ttl_t>(key,info));
            time_to_live[store_id][key]=info;
        }
    }
    /**
     * this constructor is protected.
     * @param capacity
     * @return
     */
    MemKit(long capacity){
        this->storage_capacity=capacity;
        this->storage_size=0;
        this->storage.clear();
        config=MemKitConfig::getConfigure();
        this->free_mem_size=0;
        this->total_mem_size=0;
        this->used_mem_size=0;
    }
    /**
     * if you want to load the file to memcache
     * @param filepath  the filepath
     */
     MemKit* load(String file){
        std::ifstream reader(file.c_str());
        String line,store_id,key,value;
        while((getline(reader,line))){
            /**
             * the store id,so,we need to create the store id.
             */
           if(line[0]=='[') {
               store_id = line.substr(line.find_first_of(']')+1,line.length());
           }else{
               /**
                * get the key and value
                */
               int sp=line.find_first_of(" ");
               key=line.substr(0,sp);
               value=line.substr(sp,line.find_first_of(" "));
               value=value.substr(value.find_first_not_of(" "));
               memKit_instance->put(store_id,key,value);
           }
        }
        reader.close();
        config=MemKitConfig::getConfigure();
        return memKit_instance;
    }

    /**
     * set the key's ttl
     * @param setter
     */
    void set(TTL_info setter){
        /**
         * you should set the ttl.
         */
        setter.ttl_info.putTime=this->getMillis();
        this->setTTL(setter.store_id,setter.key,setter.ttl_info);
    }

    /**
     * THE PUBLIC PART
     */
MEMKIT_PUBLIC:
    /**
     * set the mem size
     * @param ms
     */
    void setTotalMem(int ms){
        this->total_mem_size=ms;
    }
    /**
     * get the total mem
     * @return
     */
    int getTotalMem(){
        return this->total_mem_size;
    }
    /**
     * set the used mem
     * @param ms
     */
    void setUsedMem(int ms){
        this->used_mem_size=ms;
    }
    /**
     * get
     * @return
     */
    int getUsedMem(){
        return this->used_mem_size;
    }
    /**
     *
     * @param ms
     */
    void setFreeMem(int ms){
        this->free_mem_size=ms;
    }
    /**
     *
     * @return
     */
    int getFreeMem(){
        return this->free_mem_size;
    }

    /**
     * get the ttl storage
     * @return
     */
    std::map<String,std::map<String,ttl_t>> getTTLStorage(){
        return this->time_to_live;
    }
    /**
     * get the mem storage
     * @return
     */
    std::map<String,std::map<String,String>> getStorage(){
        return this->storage;
    };
    /**
     * if you want to get an instance
     * @param capacity
     * @return
     */
    static MemKit* getInstance(long capacity){
        if(memKit_instance==NULL){
            memKit_instance=new MemKit(capacity);
        }
        return memKit_instance;
    }
    /**
     * if you want to get an new instance.
     * NOTICE:THE OLD DATA WILL LOST.
     * @param capacity new capacity
     * @return  an new instance
     */
    static MemKit* getNewInstance(long capacity){
        memKit_instance=new MemKit(capacity);
        return memKit_instance;
    }
    /**
     * if you want to load file.
     * @param file
     * @return
     */
     MemKit* loadFromFile(String file){
        memKit_instance=new MemKit(atol(config->getCapacity().c_str()));
        return load(file);
    }


    /**
     * delete the item
     * @param store_id
     * @param key
     * @return
     */
    String popMem(String store_id,String key){
        std::map<String,std::map<String,String>>::iterator sit=
                this->storage.find(store_id);
        if(sit==this->storage.end()){
            return "";//nothing to delete
        }else{
            std::map<String,String>::iterator dit=
                    (*sit).second.find(key);
            if(dit==(*sit).second.end()){
                return "";
            }else{
                String p=(*dit).second;
                (*sit).second.erase(dit);
                this->storage_size-=1;
                return p;
            }
        }
        std::cout<<"rm ok"<<std::endl;
    }
    /**
     * delete the ttl item
     * @param store_id
     * @param key
     * @return
     */
    String popTm(String store_id,String key){
        std::map<String,std::map<String,ttl_t>>::iterator sit=
                this->time_to_live.find(store_id);
        if(sit==this->time_to_live.end()){
            return "";//nothing to delete
        }else{
            std::map<String,ttl_t>::iterator dit=(*sit).second.find(key);
            if(dit==(*sit).second.end()){
                return "";
            }else{
                String p=(*dit).first;
                this->time_to_live[store_id].erase(key);
                /**
                 * also,you should del the mem
                 */
                this->popMem(store_id,key);
                return p;
            }
        }
    }
    /**
     * another ttl function
     * @param store_id
     * @param key
     * @param ttl
     */
    void set(String store_id,String key,long ttl){
        TTL_info setter;
        setter.store_id=store_id;
        setter.key=key;
        setter.ttl_info.TTL=ttl;
        set(setter);
    }
    /**
     * get the ttl info
     * @param store_id
     * @param key
     * @return
     */
    TTL_info ttl(String store_id,String key){
        TTL_info info;
        if(this->exist(store_id,key)) {
            std::map<String, std::map<String, ttl_t>>::iterator itr =
                    this->time_to_live.find(store_id);
            if (itr != this->time_to_live.end()) {
                std::map<String, ttl_t>::iterator si = (*itr).second.find(key);
                if (si != (*itr).second.end()) {
                    info.store_id = store_id;
                    info.key = key;
                    info.ttl_info = (*si).second;
                }
            }
        }else{
            info.store_id=store_id;
            info.key=key;info.ttl_info.TTL=0;
        }
        return info;
    }
    /**
     * check if the store_id@key is existe
     * @param store_id
     * @param key
     * @return
     */
    bool exist(String store_id,String key){
        if(this->storage.find(store_id)==this->storage.end()){
            return false;
        }else{
            if(this->storage[store_id].find(key)==this->storage[store_id].end()){
                return false;
            }else{
                return true;
            }
        }
    }
    /**
     * check by the store id
     * @param store_id
     * @return
     */
    bool exist(String store_id){
        return this->storage.find(store_id)!=this->storage.end();
    }

    /**
     * get the leave time
     * @param store_id
     * @param key
     * @return
     */
    long getTTL(String store_id,String key){
        if(this->exist(store_id,key)){
            std::cout<<"ttl:"<<ttl(store_id,key).ttl_info.TTL<<std::endl;
            return (getMillis()-ttl(store_id,key).ttl_info.putTime);
        }else{
            std::cout<<"die!"<<std::endl;
            return 0;
        }
    }
    /**
     * get the value
     * @param store_id
     * @param key
     * @return
     */
    String get(String store_id,String key){
        std::map<String,std::map<String,String>>::iterator IdItr=this->storage.find(store_id);
        if(IdItr==this->storage.end()){/*no this storage*/
            return "null";
        }else{
            std::map<String,String>::iterator itr=(*IdItr).second.find(key);
            if(itr==(*IdItr).second.end()){
                return "null";
            }else{
                return (*itr).second;
            }
        }
    }
    /**
     * set the store_id@key=replaceTo
     * @param store_id
     * @param key
     * @param replaceTo  to value
     * @return
     */
    bool replace(String store_id,String key,String replaceTo){
        if(!this->exist(store_id,key)){
            return false;
        }
        std::map<String,std::map<String,String>>::iterator idItr=this->storage.begin();
        for(;idItr!=this->storage.end();idItr++){
            if((*idItr).first==store_id){
                std::map<String,String>::iterator kit=(*idItr).second.begin();
                for(;kit!=(*idItr).second.end();kit++){
                    if((*kit).first==key){
                        (*kit).second=replaceTo;
                        return true;
                    }
                }
            }
        }
        return false;//forever no reach here.
    }

    /**
     * rename the store id,if the new store id name is exist,just return false
     * NOTICE:YOU ALSO NEED TO UPDATE THE INFORMATION IN TTL STORAGE.
     * @param old_store_id
     * @param new_store_id
     */
    bool renameStore(String old_store_id,String new_store_id){
        if(this->exist(old_store_id)==false||this->exist(new_store_id)){
            return false;
        }
        std::map<String,std::map<String,String>>::iterator idItr=
                this->storage.begin(),dit;
        for(;idItr!=this->storage.end();idItr++){
            if((*idItr).first==old_store_id){
                std::map<String,String> tmp=(*idItr).second;
                dit=idItr;
                this->storage.erase(dit);
                this->storage.insert(std::pair<String,std::map<String,String>>(new_store_id,tmp));
                break;
            }
        }
        //update in storage
        std::map<String,std::map<String,ttl_t>>::iterator idVit=this->time_to_live.begin(),dVit;
        for(;idVit!=this->time_to_live.end();idVit++){
            if((*idVit).first==old_store_id){
                std::map<String,ttl_t> t=(*idVit).second;
                dVit=idVit;
                this->time_to_live.erase(dVit);
                this->time_to_live.insert(std::pair<String,std::map<String,ttl_t>>(new_store_id,t));
                break;
            }
        }
        return true;
    }
    /**
     * rename the key.return false means change failed
     * NOTICE:YOU ALSO NEED TO UPDATE THE INFORMATION IN TTL STORAGE.
     * @param store_id
     * @param old_key
     * @param new_key
     * @return
     */
    bool renameKey(String store_id,String old_key,String new_key){
        if(!this->exist(store_id,old_key)||this->exist(store_id,new_key)){
            return false;
        }
        std::map<String,std::map<String,String>>::iterator idItr=
                this->storage.begin();
        for(;idItr!=this->storage.end();idItr++){
            if((*idItr).first==store_id){
                std::map<String,String>::iterator kit=(*idItr).second.begin(),dit;
                for(;kit!=(*idItr).second.end();kit++){
                    if((*kit).first==old_key){
                        //find it.
                        dit=kit;
                        String v=(*kit).second;
                        (*idItr).second.erase(dit);
                        (*idItr).second.insert(std::pair<String,String>(new_key,v));
                        //update the information in ttl storage
                        std::map<String,std::map<String,ttl_t>>::iterator
                        idVit=this->time_to_live.begin();
                        for(;idVit!=this->time_to_live.end();idVit++){
                            if((*idVit).first==store_id){
                                std::map<String,ttl_t>::iterator tit=(*idVit).second.begin();
                                for(;tit!=(*idVit).second.end();tit++){
                                    if((*tit).first==old_key){
                                        //find it..
                                        ttl_t ov=(*tit).second;
                                        std::map<String,ttl_t>::iterator rit=tit;
                                        (*idVit).second.erase(rit);
                                        (*idVit).second.insert(std::pair<String,ttl_t>(new_key,ov));
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /**
     * get an random key
     * @param ikv ikv=true means you want to get complete information
     * return the store_id,key,value
     * @return
     */
    String randomKey(bool ikv=false){
        srand((unsigned)time(NULL));
        int randIndex=0;
        if(this->size()!=1){
            randIndex=(rand()%(this->size()-1));
        }
        int till=0;
        std::map<String,std::map<String,String>>::iterator idItr=this->storage.begin();
        std::map<String,String>::iterator kit;
        for(;idItr!=this->storage.end();idItr++){
            if((*idItr).second.size()!=0){
                if((*idItr).second.size()+till<randIndex){
                    till+=(*idItr).second.size();
                }else{
                    kit=(*idItr).second.begin();
                    while(till<randIndex){
                        till++;
                        kit++;
                    }
                    if(ikv){
                        return (*idItr).first+","+(*kit).first+","+(*kit).second;
                    }
                    return (*kit).first;
                }
            }
        }
        return "null";
    }

    /**
     * dump the storage to disk
     * @param clear true means you want to clear the storage after dump.
     * @param file file to dump.
     */
    void dump(String file,bool clear=false/*is clear.*/){
        if(this->storage_size==0){
            std::cout<<"nothing to dump"<<std::endl;
            return;
        }
        /**
         * open the file.
         */
        std::ofstream writer(file.c_str());
        /**
         * the flush format will like this=>
         * ---------------------------------
         * store_id key value
         * ---------------------------------
         */
        std::map<String,std::map<String,String>>::iterator idItr=this->storage.begin();
        std::map<String,String>::iterator kvItr;
        for(;idItr!=this->storage.end();idItr++){
            writer<<"[store]"<<(*idItr).first<<std::endl;
            kvItr=(*idItr).second.begin();
            for(;kvItr!=(*idItr).second.end();kvItr++){
                writer<<(*kvItr).first<<" "<<(*kvItr).second<<std::endl;
            }
        }
        writer.flush();
        writer.close();
        /**
         * you say you want to clear the storage
         */
        if(clear==true){
            this->flushAll();
        }
    }
    /**
     * append a new value.
     * @param store_id
     * @param key
     * @param append_value
     * @param replace true means just replace if the key exist
     */
    void append(String store_id,String key,String append_value,bool replace=false){
        if(replace==true){
            this->put(store_id,key,append_value);
        }else{//just append.
            if(this->storage_size+1<this->storage_capacity) {
                /**
                 * check if the store id exist
                 */
                std::map<String, std::map<String, String>>::iterator IDItr = this->storage.find(store_id);
                /**
                 * this is a new key.
                 */
                if (IDItr == this->storage.end()) {
                    /**
                     * create an new id.
                     */
                    std::map<String, String> newMap;
                    newMap.clear();
                    newMap.insert(std::pair<String,String>(key,append_value));
                    this->storage.insert(std::pair<String, std::map<String, String>>(store_id, newMap));
                    /**
                     * update the size
                     */
                    this->storage_size = this->storage_size+1;
                }
                else {/*existed*/
                    /**
                     * check if the key existed
                     */
                    std::map<String,String>::iterator searchItr=(*IDItr).second.find(key);
                    if(searchItr==(*IDItr).second.end()){/*not exist*/
                        (*IDItr).second.insert(std::pair<String,String>(key,append_value));
                        this->storage_size=this->storage_size+1;
                    }
                    else{/*exist,just override it*/
                        (*searchItr).second+=append_value;
                    }
                }
            }
            else {
                std::cout<<"out of capacity,refuse to insert."<<std::endl;
            }
            TTL_info myTTL;
            myTTL.store_id=store_id;
            myTTL.key=key;
            myTTL.ttl_info.putTime=getMillis();
            myTTL.ttl_info.TTL=atol(config->getTTL().c_str());
            set(myTTL);
        }
    }

    /**
     * put a key into mem
     * @param store_id  you should offer an id
     * @param key  the key,if esist,just override
     * @param value  the value
     * @return the current storage size
     */
    long put(String store_id,String key,String value){
        if(this->storage_size+1<this->storage_capacity) {
            TTL_info myTTL;
            myTTL.store_id=store_id;
            myTTL.key=key;
            myTTL.ttl_info.putTime=getMillis();
            myTTL.ttl_info.TTL=atol(config->getTTL().c_str());
            set(myTTL);
            /**
             * check if the store id exist
             */
            std::map<String, std::map<String, String>>::iterator IDItr = this->storage.find(store_id);
            /**
             * this is a new key.
             */
            if (IDItr == this->storage.end()) {
                /**
                 * create an new id.
                 */
                std::map<String, String> newMap;
                newMap.clear();
                newMap.insert(std::pair<String,String>(key,value));
                this->storage.insert(std::pair<String, std::map<String, String>>(store_id, newMap));
                /**
                 * update the size
                 */
                this->storage_size = this->storage_size+1;
            }
            else {/*existed*/
                /**
                 * check if the key existed
                 */
                std::map<String,String>::iterator searchItr=(*IDItr).second.find(key);
                if(searchItr==(*IDItr).second.end()){/*not exist*/
                    (*IDItr).second.insert(std::pair<String,String>(key,value));
                    this->storage_size=this->storage_size+1;
                    return this->storage_size;
                }
                else{/*exist,just override it*/
                    (*searchItr).second=value;
                    return this->storage_size;
                }
            }
        }
         /**
          * else,just refuse insert
          */
        else {
            std::cout<<"out of capacity,refuse to insert."<<std::endl;
            return this->storage_size;
        }
    }
    /**
     * clear the storage
     * @param store_id
     */
    void flushStore(String store_id){
        std::map<String,std::map<String,String>>::iterator idItr=this->storage.find(store_id);
        //do not forget update the size
        this->storage_size-=(*idItr).second.size();
        if(idItr!=this->storage.end()){
            (*idItr).second.clear();
        }//else,just return
    }
    /**
     * flush all.
     */
    void flushAll(){
        this->storage.clear();
        this->storage_size=0;
    }

    /**
     * get the current size
     * @return
     */
    long size(){
        return this->storage_size;
    }
    /**
     * get the capacity
     * @return
     */
    long capacity(){
        return this->storage_capacity;
    }
    /**
     * re-set the capacity.if the new size <current size.lost datas
     * else,none.
     * @param new_sz
     * @return  the losts data count
     */
    long setCapacity(long new_sz){
        if(new_sz!=this->storage_capacity){
            if(new_sz<this->storage_size){
                long distance=this->storage_size-new_sz;
                std::map<String,std::map<String,String>>::iterator itrID=
                        this->storage.begin();
                std::map<String,String>::iterator itrTraversal,itrErase;
                int del=0;
                for(;itrID!=this->storage.end()&&del<distance;itrID++){
                    if(del+(*itrID).second.size()<distance){
                        del+=(*itrID).second.size();
                        (*itrID).second.clear();
                    }else{
                        long need=distance-del;
                        itrTraversal=(*itrID).second.begin();
                        for(int i=0;i<need;++i){
                            itrErase=itrTraversal;
                            (*itrID).second.erase(itrErase);
                            itrTraversal++;
                            del++;
                        }
                    }
                }
                this->storage_capacity=new_sz;
                this->storage_size=new_sz;
                return distance;
            }else{
                this->storage_capacity=(new_sz);
                return 0;
            }
        }else{
            return 0;
        }
    }
};

#endif //MEMKIT_MEMKIT_H
