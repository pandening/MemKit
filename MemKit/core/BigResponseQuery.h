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

#ifndef MEMKIT_BIGRESPONSEQUERY_H
#define MEMKIT_BIGRESPONSEQUERY_H

#include <vector> /*for vector*/

#include "../MemKitThreadFactory/MemKitThreadFactory.h"
#include "MemKit.h"

/**
 * 1.get keys list
 * 2.get stores list
 * 3.put lists
 */
class BigResponseQuery:public MemKitThreadFactory{
private:
    /**
     * "ks"->get keys list
     * "ss"->get stores list
     * "ps"->put lists key-value
     */
    std::string query_type;
    /**
     * the param
     */
    String kvsList;
    /**
     * the store_id
     */
    String store_id;
    /**
     * result vector
     */
    std::vector<std::string> vec;
    /**
     * the puts tag
     */
    String putResponse;
    /**
     * the memkit
     */
    MemKit* Mem;

protected:
    /**
     * get keys list
     * @param store_id you want to get the store id 's keys list
     * @return
     */
    std::vector<std::string> getKeysList(String store_id=""){
        std::vector<String> res;
        res.clear();
        if(this->Mem->size()==0){
            return res;
        }
        if(store_id=="") {
            std::map<String, std::map<String, String>>::iterator idItr =
                    this->Mem->getStorage().begin();
            std::map<String, String>::iterator kit;
            for (; idItr != this->Mem->getStorage().end(); idItr++) {
                kit = (*idItr).second.begin();
                for (; kit != (*idItr).second.end(); kit++) {
                    res.push_back((*kit).first);
                }
            }
        }else{
            //if this store exist
            std::map<String, std::map<String, String>>::iterator idItr =
                    this->Mem->getStorage().begin();
            std::map<String, String>::iterator kit;
            for(;idItr!=this->Mem->getStorage().end();idItr++){
                if((*idItr).first==store_id){
                    kit=(*idItr).second.begin();
                    for(kit;kit!=(*idItr).second.end();kit++){
                        res.push_back((*kit).first);
                    }
                    break;//i think the store is only one
                }
            }
        }
        return res;
    }
    /**
     * get stores list
     * @return
     */
    std::vector<std::string> getStoresList(){
        std::vector<String> res;
        res.clear();
        if(this->Mem->size()==0){
            return res;
        }
        std::map<String,std::map<String,String>>::iterator idItr=
                this->Mem->getStorage().begin();
        for(;idItr!=this->Mem->getStorage().end();idItr++){
            res.push_back((*idItr).first);
        }
        return res;
    }
    /**
     * you should give me the params like this=>
     * puts sid,key,value sid,key,value ....
     * @param kvs
     */
    void putKVLists(String kvs){
        std::vector<String> params=MemKitUtils::split(kvs," ");
        if(params[0]!="ps"||params.size()<=1){
            this->putResponse="fail";
            return;
        }
        std::vector<String> id_k_v;
        int index=1;//0 is the command
        int succeed=0;//total succeed
        for(;index!=params.size();index++){
            id_k_v.clear();
            id_k_v=MemKitUtils::split(params[index],",");
            if(id_k_v.size()==3){
                succeed++;
                this->Mem->put(id_k_v[0],id_k_v[1],id_k_v[2]);
            }
        }
        this->putResponse=MemKitUtils::parseString(succeed);
    }

public:
    /**
     * you should offer the loop
     * @param loop
     * @param type
     * @param kvs puts
     * @param store_id store id to get
     * @return
     */
    BigResponseQuery(bool loop, std::string type,String kvs="",String store_id="") : MemKitThreadFactory(loop) {
        this->query_type=type;
        Mem=MemKit::getInstance(0);
        this->kvsList=kvs;
        this->store_id=store_id;
    }
    /**
     * override
     */
    void execute(){
        if(this->query_type=="ks"){
            this->vec=this->getKeysList(store_id);
        }else if(this->query_type=="ss"){
            this->vec=this->getStoresList();
        }else if(this->query_type=="ps"){
            this->putKVLists(this->kvsList);
        } else{
            this->vec.clear();
            std::cout<<"error param to query"<<std::endl;
        }
        std::cout<<"--big query done---"<<std::endl;
    }
    /**
     * get the query list
     * @return
     */
    std::vector<std::string> getResult(){
        return this->vec;
    }
    /**
     * if you recv a string =>"fail" means put error
     * or you will recv a number means how many item put ok~
     * @return
     */
    String getPutResult(){
        return this->putResponse;
    }
};

#endif //MEMKIT_BIGRESPONSEQUERY_H
