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
#ifndef MEMKIT_MEMSNAPSHOT_H
#define MEMKIT_MEMSNAPSHOT_H

#include <string>
#include "../MemTimer/TimerFactory.h"
#include "../core/MemKit.h"


typedef struct snapshot{
    String id,key,value;
    snapshot(String i,String k,String v):id(i),key(k),value(v){}
}snapshot_t;

class Mem_Snapshot: public BaseTimer{
private:
    /**
     * the snapshot
     */
    std::vector<snapshot_t> SnapshotVec;
    /**
     * the snap size request
     */
    int snapSize;
    /**
     * actual size to generator
     */
    int actual_size;
    /**
     * the memkit
     */
    MemKit* Mem;

protected:
    /**
     * the read counter
     * @param act
     */
    void puppet(int act){
        std::ifstream reader(Configure->getDumpFile().c_str());
        String line,store_id,key,value;
        int counter=0;
        while((getline(reader,line))&&counter!=act){
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
                this->SnapshotVec.push_back(snapshot(store_id,key,value));
                counter++;
            }
        }
        reader.close();
    }
public:
    /**
     * it's time to do it
     * @param start
     * @param delay
     * @param last=-1 means forever
     * @param sSize give me the size
     * @return
     */
    Mem_Snapshot(long start, long delay, long last,int sSize) : BaseTimer(start, delay, last) {
        this->SnapshotVec.clear();
        this->snapSize=sSize;
        this->actual_size=0;
        this->Mem=MemKit::getInstance(0);
    }
    /**
     * override
     */
    void execute(){
        this->Mem=MemKit::getInstance(0);
        this->SnapshotVec.clear();
        if(this->Mem->size()>=this->snapSize){
            this->actual_size=this->snapSize;
        }else{
            if(this->Mem->size()==0){
                this->actual_size=0;
                return;
            }else{
                this->actual_size=this->Mem->size();
            }
        }
        this->puppet(this->actual_size);
    }
    /**
     * get the actual snap shot size
     * @return
     */
    int getSnapShotSize(){
        return this->SnapshotVec.size();
    }
    /**
     * get the snapshot string
     * @return
     */
    std::vector<snapshot_t> getSnapShot(){
        return this->SnapshotVec;
    }
};

#endif //MEMKIT_MEMSNAPSHOT_H
