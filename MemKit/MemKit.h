//
// Created by hujian on 16-9-5.
//
#ifndef MEMKIT_MEMKIT_H
#define MEMKIT_MEMKIT_H

#define MEMKIT_PRIVATE private
#define MEMKIT_PUBLIC  public
#define MEMKIT_PROTECTED protected
#define DEFAULT_CAPACITY 1000 //the default storage capacity is 1000 items
#define OUT_OF_TTL_BOUNDER 1000*60*60
#define DEFAULT_TTL 1000*60*60 /*one hour*/
#define DEFAULT_DUMP_FILE "./dump.txt"

#include<map> /*for map*/
#include<string> /*for string*/
#include <string.h>
#include <iostream> /*for cout*/
#include <time.h>
#include <fstream> /*for I/O*/

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
    long leave;
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
               int sp=line.find_first_of(' ');
               key=line.substr(0,sp);
               value=line.substr(sp,line.length());
               memKit_instance->put(store_id,key,value);
               std::cout<<store_id<<" "<<key<<" "<<value<<std::endl;
           }
        }
        reader.close();
        return memKit_instance;
    }

    /**
     * THE PUBLIC PART
     */
MEMKIT_PUBLIC:
    /**
     * get the ttl storage
     * @return
     */
    std::map<String,std::map<String,ttl_t>> getTTLStorage(){
        return this->time_to_live;
    }
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
        memKit_instance=new MemKit(DEFAULT_CAPACITY);
        return load(file);
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
                (*sit).second.erase(dit);
                return p;
            }
        }
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
        setter.ttl_info.leave=time((time_t*)NULL)-setter.ttl_info.putTime;
        this->setTTL(setter.store_id,setter.key,setter.ttl_info);
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
        setter.ttl_info.leave=0;
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
        std::map<String,std::map<String,ttl_t>>::iterator itr=
                this->time_to_live.find(store_id);
        if(itr!=this->time_to_live.end()){
            std::map<String,ttl_t>::iterator si= (*itr).second.find(key);
            if(si!=(*itr).second.end()){
                info.store_id=store_id;
                info.key=key;
                info.ttl_info=(*si).second;
            }
        }
        return info;
    }
    /**
     * get the leave time
     * @param store_id
     * @param key
     * @return
     */
    long getTTL(String store_id,String key){
        return ttl(store_id,key).ttl_info.leave;
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
            return NULL;
        }else{
            std::map<String,String>::iterator itr=(*IdItr).second.find(key);
            if(itr==(*IdItr).second.end()){
                return NULL;
            }else{
                return (*itr).second;
            }
        }
    }
    /**
     * dump the storage to disk
     * @param clear true means you want to clear the storage after dump.
     */
    void dump(bool clear=false/*is clear.*/){
        if(this->storage_size==0){
            std::cout<<"nothing to dump"<<std::endl;
            return;
        }
        /**
         * create the file name according to the time stamp
         */
        char buf[1024];
        memset(buf,0, sizeof(buf));
        sprintf(buf,"./%ld.txt",this->getMillis());
        /**
         * open the file.
         */
        std::ofstream writer(buf);
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
         * you say you want to clear the starage
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
            myTTL.ttl_info.leave=0;
            myTTL.ttl_info.putTime=getMillis();
            myTTL.ttl_info.TTL=DEFAULT_TTL;
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
            myTTL.ttl_info.leave=0;
            myTTL.ttl_info.putTime=getMillis();
            myTTL.ttl_info.TTL=DEFAULT_TTL;
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
            TTL_info myTTL;
            myTTL.store_id=store_id;
            myTTL.key=key;
            myTTL.ttl_info.leave=0;
            myTTL.ttl_info.putTime=getMillis();
            myTTL.ttl_info.TTL=DEFAULT_TTL;
            set(myTTL);
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
