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
#ifndef MEMKIT_REFRESHMEM_H
#define MEMKIT_REFRESHMEM_H

#include "MemKit.h"

#define os std::cout
#define is std::cin
#define el std::endl
/**
 * this class will check the mem,then remove the item (item's ttl<0)
 */
class RefreshMem{
private:
    /**
     * the fresh time
     */
    long time_to_refresh;
    /**
     * the mem,this ok because if the instance is not null,the new capacity is
     * not work out~
     */
    MemKit* Mem;
    std::map<String,std::map<String,ttl_t>> ttl_Map;
    /**
     * the puppet of this class
     */
    void puppet(){
        /**
         * forever
         */
        while(true){
            ttl_Map=Mem->getTTLStorage();
            long del=0;
            sleep(this->time_to_refresh);
            os<<"Timer:total size to scan=>"<<ttl_Map.size()<<el;
            if(ttl_Map.size()==0){
                continue;
            }
            std::map<String,std::map<String,ttl_t>>::iterator sit=this->ttl_Map.begin();
            std::map<String,ttl_t>::iterator kit;
            for(;sit!=ttl_Map.end();sit++){
                for(kit=(*sit).second.begin();kit!=(*sit).second.end();kit++){
                    if(time((time_t*)NULL)-(*kit).second.putTime>=(*kit).second.TTL){
                        Mem->popTm((*sit).first,(*kit).first);
                        del++;
                    }
                }
            }
            os<<"Total delete of time out items:"<<del<<el;
        }
    }
public:
    /**
     * the constructor
     * @param fTime
     * @return
     */
    RefreshMem(long fTime){
        Mem=MemKit::getInstance(1234);
        this->time_to_refresh=fTime;
        this->puppet();
    }
};
#endif //MEMKIT_REFRESHMEM_H
