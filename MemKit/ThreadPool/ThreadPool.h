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

#ifndef MEMKIT_THREADPOOL_H
#define MEMKIT_THREADPOOL_H

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <thread>

#define is std::cin
#define os std::cout
#define el std::endl
typedef std::string String;
/**
 * this is the runnable object,it's a function
 */
typedef std::function<void()> Runnable;

/**
 * thread pool
 */
class ThreadPool{
private:
    /**
     * the thread pool size
     */
    int poolSize;
    /**
     * the thread pool's status
     */
    bool running;
    /**
     * the runnable  thread vector
     */
    std::vector<std::thread> ThreadVec;
    /**
     * the  task vector
     */
    std::vector<Runnable> TaskVec;

protected:
    /**
     * run a job
     */
    void executor(){
        if(running){
            Runnable task=take();
            if(task==NULL){
                return;
            }else{
                task();
            }
        }
    }
    /**
     * get a task
     * @return
     */
    Runnable take(){
        if(this->TaskVec.size()==0){
            return NULL;
        }
        Runnable task=this->TaskVec.back();
        this->TaskVec.pop_back();
        return task;
    }
    /**
     * start to run
     */
    void start(){
        this->running=true;
        for(int i=0;i<this->TaskVec.size();i++){
            this->ThreadVec.push_back(std::thread(&ThreadPool::executor,this));
        }
    }
public:
    /**
     * you should offer the pool's size
     * @param size
     * @return
     */
    ThreadPool(int size=10){
        this->poolSize=size;
        this->running=false;
    }
    /**
     * add a task to run
     * @param task
     */
    void add(const Runnable&task){
        if(this->ThreadVec.size()==this->poolSize){
            return;
        }
        this->TaskVec.push_back(task);
        this->start();
    }

    /**
     * stop
     */
    void stop(){
        this->running=false;
        for(int i=0;i<this->poolSize;i++){
            this->ThreadVec[i].join();
        }
    }
    /**
     * get the thread pool's status
     * @return
     */
    bool status(){
        return this->running;
    }
    /**
     * get the pool size
     * @return
     */
    int size(){
        return this->poolSize;
    }
    /**
     * get the task size
     * @return
     */
    int taskSize(){
        return this->TaskVec.size();
    }
    /**
     * if you want to re-set the thread pool's size
     * @param size
     */
    void setSize(int size){
        if(this->poolSize<=size){
            return;
        }
        this->poolSize=size;
    }
};

#endif //MEMKIT_THREADPOOL_H
