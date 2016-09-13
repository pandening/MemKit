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

#include "../MemKitUtils/MemKitUtils.h"
#include "../conf/Configure.h"
#include "../core/BigResponseQuery.h"
#include "../conf/ComputerMem.h"

#define os std::cout
#define el std::endl
#define is std::cin
#define DEBUG 0   /*for debug*/

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
     * the config
     */
    MemKitConfig* config;
    /**
     * show the usage
     */
    void usage(){
        os<<"\tload      [filePath]                                load cache from file,the old mem will be cleared."<<el;
        os<<"\tset       [store id][key][time to live(mills)]      set ttl for store@key"<<el;
        os<<"\tttl       [store id][key]                           get the ttl of store@key"<<el;
        os<<"\tget       [store id][key]                           get the value"<<el;
        os<<"\tdump      [file][true/false]                        dump to disk.(true means flush the cache)"<<el;
        os<<"\tappend    [store id][key][append value]             append a value"<<el;
        os<<"\tput       [store id][key][value]                    put a key-value"<<el;
        os<<"\tps        [store_id,key,value]list                  put a list"<<el;
        os<<"\tks        [store_id->default is null]               get the keys list"<<el;
        os<<"\tss                                                  get the stores list"<<el;
        os<<"\tflush     [store]                                   flush the storage"<<el;
        os<<"\tflusha                                              flush all"<<el;
        os<<"\tinfo                                                show the size"<<el;
        os<<"\tsetc      [new capacity]                            re-set the capacity"<<el;
        os<<"\trm        [store id][key]                           delete the key-value"<<el;
        os<<"\texist     [store id][key]                           judge if the id@key exist"<<el;
        os<<"\texistid   [store id]                                just check the store id"<<el;
        os<<"\texit                                                exit~"<<el;
        os<<"\thh                                                  alive check"<<el;
        os<<"\trs        [old store id][new store id]              rename a store id"<<el;
        os<<"\trk        [store id][old key][new key]              rename a key"<<el;
        os<<"\tre        [store id][key][replace to]               replace the value"<<el;
        os<<"\trd                                                  get an random key"<<el;
        os<<"\tmem                                                 get the mem info.<total,used,free>"<<el;
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
        config=MemKitConfig::getConfigure();
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
                case 'm':{//get the mem
                    if(this->__instance->getTotalMem()==0){
                        ComputerMem* computerMem=new ComputerMem;
                    }
                    os<<this->__instance->getTotalMem()<<"Mbs,"<<this->__instance->getUsedMem()
                      <<"Mbs,"<<this->__instance->getFreeMem()<<"Mbs"<<el;
                    break;
                }
                case 'k':{//ks
                    store_id.clear();
                    if(splitVec.size()==2){
                        store_id=splitVec[1];
                    }
                    BigResponseQuery* big=new BigResponseQuery(false,"ks","",store_id);
                    big->run();
                    std::vector<String> key_list;
                    key_list=big->getResult();
                    std::vector<String>::iterator vit=key_list.begin();
                    for(;vit!=key_list.end();vit++){
                        os<<"\t"<<*vit<<el;
                    }
                    break;
                }
                case 'h':{
                    os<<"alive"<<el;
                    break;
                }
                case 'r':{
                    if(command=="rs"){//rename a store
                        String old_sid,new_sid;
                        old_sid=splitVec[1];
                        new_sid=splitVec[2];
                        bool res=this->__instance->renameStore(old_sid,new_sid);
                        if(res==true){
                            os<<"rename ok"<<el;
                        }else{
                            os<<"rename fail"<<el;
                        }
                    }else if(command=="rk"){//rename a key
                        String old_key,new_key;
                        store_id=splitVec[1];
                        old_key=splitVec[2];
                        new_key=splitVec[3];
                        bool res=this->__instance->renameKey(store_id,old_key,new_key);
                        if(res==true){
                            os<<"rename ok"<<el;
                        }else{
                            os<<"rename fail"<<el;
                        }
                    }else if(command=="rm") {
                        store_id = splitVec[1];
                        key = splitVec[2];
                        if (DEBUG) {
                            os << "delete store[" << store_id << "] key[" << key << "]" << el;
                        }
                        this->__instance->popMem(store_id, key);
                        os << "deleted" << el;
                    }else if(command=="re"){
                        store_id=splitVec[1];
                        key=splitVec[2];
                        value=splitVec[3];
                        bool res=this->__instance->replace(store_id,key,value);
                        if(res==true){
                            os<<"replace ok"<<el;
                        }else{
                            os<<"replace fail"<<el;
                        }
                    }else if(command=="rd"){
                        key=this->__instance->randomKey(true);
                        os<<"random:"<<key<<el;
                    }
                    break;
                }
                case 'e':{
                    cmd=splitVec[0];
                    if(cmd=="exit") {
                        os << "\tBye~" << el;
                        exit(0);
                    }else if(cmd=="exist"){//you just want to check
                        store_id=splitVec[1];
                        key=splitVec[2];
                        bool check=this->__instance->exist(store_id,key);
                        if(check){
                            os<<"true"<<el;
                        }else{
                            os<<"false"<<el;
                        }
                        break;
                    }else{//exist id
                        store_id=splitVec[1];
                        bool check=this->__instance->exist(store_id);
                        if(check){
                            os<<"true"<<el;
                        }else{
                            os<<"false"<<el;
                        }
                        break;
                    }
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
                        os<<"set capacity ok"<<el;
                        if(DEBUG){
                            os<<"new capacity["<<cap<<"]"<<el;
                        }
                        break;

                    }else if(cmd=="ss"){//get the list
                        BigResponseQuery* big=new BigResponseQuery(false,"ss");
                        big->run();
                        std::vector<String> store_list;
                        store_list=big->getResult();
                        std::vector<String>::iterator vit=store_list.begin();
                        for(;vit!=store_list.end();vit++){
                            os<<"\t"<<*vit<<el;
                        }
                        break;
                    }
                    else {
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
                    if(splitVec.size()==3) {
                        clear = splitVec[2];
                        String file = splitVec[1];
                        if (clear == "true") {
                            this->__instance->dump(file, true);
                        } else {
                            this->__instance->dump(file, false);
                        }
                    }else if(splitVec.size()==2){
                        String file=splitVec[1];
                        this->__instance->dump(file);
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
                    if(splitVec[0]=="put") {
                        store_id = splitVec[1];
                        key = splitVec[2];
                        value = splitVec[3];
                        /**
                         * action
                         */
                        this->__instance->put(store_id, key, value);
                        os << "\tput ok~" << el;
                        if (DEBUG) {
                            os << "store_id[" << store_id << "] key[" << key << "] value[" << value << "]" << el;
                        }
                    }else if(splitVec[0]=="ps"){
                        BigResponseQuery* big=new BigResponseQuery(false,"ps",line);
                        big->run();
                        os<<"->"<<big->getPutResult()<<el;
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
                case 'i': {
                    /**
                     * ok,this info will contains many information
                     * capacity,size,ip,port,name
                     */
                    os << "\tcapacity:" << this->__instance->capacity()
                       << " size:" << this->__instance->size()<<" ip:"
                       <<config->getIP()<<" port:"<<config->getPort()
                       <<" name:"<<config->getName()<<el;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
};

#endif //MEMKIT_CONSOLERUNNER_H
