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

#ifndef MEMKIT_MEMKITTHREADFACTORY_H
#define MEMKIT_MEMKITTHREADFACTORY_H

#include <thread>

/**
 * a little thread class.
 */
class MemKitThreadFactory{
private:
    /**
     * if you want to loop,just assign true to this variable
     */
    bool is_loop;
protected:
    /**
     * the puppet,set up the job.
     */
    void puppet(){
        if(this->is_loop==true) {
            while (true) {
                this->execute();
            }
        }else{
            this->execute();
        }
    }
public:
    /**
     * loop==true means you want to do the execute job forever.
     * @param loop
     * @return
     */
    MemKitThreadFactory(bool loop){
        this->is_loop=loop;
    }
    /**
     * start thread
     */
    void run(){
        std::thread Loop(&MemKitThreadFactory::puppet,this);
        //Loop.detach();
        Loop.join();
    }
    /**
     * please implement this function to do thing~
     */
    virtual  void execute(){
        //todo
    }
};

#endif //MEMKIT_MEMKITTHREADFACTORY_H
