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

#ifndef MEMKIT_MEMKITAUTODUMPHANDLER_H
#define MEMKIT_MEMKITAUTODUMPHANDLER_H

#include "../core/MemKit.h"
#include "../conf/Configure.h"
/**
 * auto dump yo file.the memkit will rebuild the memcached
 * according to the dump file
 */
class AutoDumpHandler{
private:
    /**
     * time to dump,I think the time should be longer than 1 minute
     */
    int time_to_dump;
    /**
     * dump file
     */
    /**
     * MemKit
     */
    MemKit* memKit;
    /**
     * the config
     */
    MemKitConfig* config;
protected:
    /**
     * the puppet.
     */
    void puppet(){
        while(true){
            sleep(time_to_dump);
            memKit->dump(config->getDumpFile());
            std::cout<<"dump file ok"<<std::endl;
        }
    }
public:
    AutoDumpHandler(){
        config=MemKitConfig::getConfigure();
        memKit=MemKit::getInstance(0);
        time_to_dump=atoi(config->getAutoDumpTime().c_str());
        this->puppet();
    }
};

#endif //MEMKIT_MEMKITAUTODUMPHANDLER_H
