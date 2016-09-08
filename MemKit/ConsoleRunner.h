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
#ifndef MEMKIT_CONSOLERUNNER_H
#define MEMKIT_CONSOLERUNNER_H
#include <vector> /*for vector*/

#include "MemKitUtils.h"

#define os std::cout
#define el std::endl
#define is std::cin
#define DEBUG 0

/**
 * run on console
 */
class ConsoleRunner{
private:
    /**
     * the instance
     */
    MemKit* __instance;
    /**
     * show the usage
     */
    void usage(){
        os<<"\tload      [filePath]                                load cache from file,the old mem will be cleared."<<el;
        os<<"\tset       [store id][key][time to live(mills)]      set ttl for store@key"<<el;
        os<<"\tttl       [store id][key]                           get the ttl of store@key"<<el;
        os<<"\tget       [store id][key]                           get the value"<<el;
        os<<"\tdump      [true/false]                              dump to disk.(true means flush the cache)"<<el;
        os<<"\tappend    [store id][key][append value]             append a value"<<el;
        os<<"\tput       [store id][key][value]                    put a key-value"<<el;
        os<<"\tflush     [store]                                   flush the storage"<<el;
        os<<"\tflusha                                              flush all"<<el;
        os<<"\tinfo                                                show the size"<<el;
        os<<"\tsetc      [new capacity]                            re-set the capacity"<<el;
        os<<"\trm        [store id][key]                           delete the key-value"<<el;
        os<<"\texit                                                exit~"<<el;
    }
    /**
     * like trim
     * @param source
     * @return
     */
    String trim(String source){
        if(source.empty()){
            return source;
        }
        source.erase(0,source.find_first_not_of(" "));
        source.erase(source.find_last_not_of(" ")+1);
        return source;
    }
    /**
     * the split
     * @param source
     * @param del
     * @return
     */
    std::vector<String> split(String source,String del)
    {
        source=this->trim(source);
        std::vector<String> vec;
        size_t  last=0;
        size_t  index=source.find_first_of(del,last);
        while(index!=std::string::npos){
            vec.push_back(source.substr(last,index-last));
            last=index+1;
            index=source.find_first_of(del,last);
        }
        if(index-last>0){
            vec.push_back(source.substr(last,index-last));
        }
        return vec;
    }

public:
    /**
     * the constructor
     * @param capacity the capacity
     * @param isNew true means new instance
     * @return
     */
    ConsoleRunner(long capacity,bool isNew=false/*true means new one*/){
        if(isNew==true){
            this->__instance=MemKit::getNewInstance(capacity);
        }else{
            this->__instance=MemKit::getInstance(capacity);
        }
    }
    /**
     * run~
     */
    void run() {
        /**
         * show the suage
         */
        this->usage();
        String line, store_id, key, value, cmd, file, ttl, cap, clear, newV,command;
        std::vector<String> splitVec;
        char buf[1024];
        while (true) {
            memset(buf, 0, sizeof(buf));
            is.getline(buf, 1024);
            line = buf;
            splitVec.clear();
            splitVec=this->split(buf," ");
            if(splitVec.empty()){
                /**
                 * run again
                 */
                continue;
            }
            if(DEBUG){
                std::vector<std::string>::iterator vit=splitVec.begin();
                for(;vit!=splitVec.end();vit++){
                    std::cout<<"-------->["<<*vit<<"]"<<std::endl;
                }
            }
            command=splitVec[0];
            switch (command[0]) {
                case 'r':{
                    store_id=splitVec[1];
                    key=splitVec[2];
                    if(DEBUG) {
                        os << "delete store[" << store_id << "] key[" << key << "]" << el;
                    }
                    this->__instance->popMem(store_id,key);
                    break;
                }
                case 'e':{
                    os<<"\tBye~"<<el;
                   exit(0);
                }
                case 'l': {//load from file
                    cmd = splitVec[0];
                    if (cmd != "load"&&cmd != "LOAD") {
                        os << "\tError input." << el;
                        if(DEBUG){
                            os<<"cmd:["<<cmd<<"]"<<el;
                        }
                        this->usage();
                        break;
                    } else {
                        /**
                         * get the file
                         */
                        file = splitVec[1];
                        /**
                         * action
                         */
                        this->__instance = this->__instance->loadFromFile(file);
                        if(DEBUG){
                            os<<"file:["<<file<<"]"<<el;
                        }
                        break;
                    }
                }
                    /**
                     * 1. set the ttl
                     * 2. set the capacity
                     */
                case 's': {
                    cmd = splitVec[0];
                    if(DEBUG){
                        os<<"cmd:["<<cmd<<"]"<<el;
                    }
                    if (cmd=="set") {
                        /**
                         * get the store id and the key and the ttl
                         */
                        store_id = splitVec[1];
                        key = splitVec[2];
                        ttl = splitVec[3];
                        if(DEBUG){
                            os<<"store_id["<<store_id<<"] key["<<key<<"] ttl["<<ttl<<"]"<<el;
                        }
                        /**
                         * action
                         */
                        this->__instance->set(store_id, key, atol(ttl.c_str()));
                        break;

                    } else if (cmd == "setc" || cmd == "SETC") {
                        /**
                         * get the new capacity
                         */
                        cap = splitVec[1];
                        /**
                         * action
                         */
                        this->__instance->setCapacity(atol(cap.c_str()));
                        if(DEBUG){
                            os<<"new capacity["<<cap<<"]"<<el;
                        }
                        break;

                    } else {
                        os << "\t Error input." << el;
                        this->usage();
                        break;
                    }
                }
                case 't': {//get the ttl
                    store_id = splitVec[1];
                    key = splitVec[2];
                    /**
                     * action
                     */
                    os << "\t" << this->__instance->getTTL(store_id, key) << el;
                    if(DEBUG){
                        os<<"store_id["<<store_id<<"] key["<<key<<"]"<<el;
                    }
                    break;
                }
                case 'g': {//get a value
                    store_id =splitVec[1];
                    key = splitVec[2];
                    /**
                     * action
                     */
                    os << "\t" << this->__instance->get(store_id, key) << el;
                    if(DEBUG){
                        os<<"store_id["<<store_id<<"] key["<<key<<"]"<<el;
                    }
                    break;

                }
                case 'd': {//dump
                    clear = splitVec[1];
                    if (clear == "true") {
                        this->__instance->dump(true);
                    } else {
                        this->__instance->dump(false);
                    }
                    os << "\tdump ok~" << el;
                    if(DEBUG){
                       os<<"flag["<<clear<<"]"<<el;
                    }
                    break;
                }
                case 'a': {
                    store_id = splitVec[1];
                    key = splitVec[2];
                    newV = splitVec[3];
                    /**
                     * action
                     */
                    this->__instance->append(store_id, key, newV);
                    os << "\tappend ok~" << el;
                    if(DEBUG){
                        os<<"store_id["<<store_id<<"] key["<<key<<"]"<<"new value["<<newV<<"]"<<el;
                    }
                    break;
                }
                case 'p': {//put
                    store_id = splitVec[1];
                    key = splitVec[2];
                    value = splitVec[3];
                    /**
                     * action
                     */
                    this->__instance->put(store_id, key, value);
                    os << "\tput ok~" << el;
                    if(DEBUG){
                        os<<"store_id["<<store_id<<"] key["<<key<<"] value["<<value<<"]"<<el;
                    }
                    break;
                }
                case 'f': {
                    cmd = splitVec[0];
                    if(DEBUG){
                        os<<"cmd["<<cmd<<"]"<<el;
                    }
                    if (cmd == "flush") {//flush
                        /**
                         * get the flush store id
                         */
                        store_id = splitVec[1];
                        /**
                         * action
                         */
                        this->__instance->flushStore(store_id);
                        os << "\tflush store ok~" << el;
                        if(DEBUG){
                            os<<"store_id["<<store_id<<el;
                        }
                        break;
                    } else {//flush all
                        this->__instance->flushAll();
                        os << "\tflush all ok~" << el;
                        break;
                    }
                }
                case 'i': {//show the size
                    os << "\tcapacity:" << this->__instance->capacity() << el << "\tsize:" << this->__instance->size()
                       << el;
                    break;
                }
                default: {
                    os << "\tunknown command!" << el;
                    this->usage();
                    break;
                }
            }
        }
    }
};

#endif //MEMKIT_CONSOLERUNNER_H
