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

#ifndef MEMKIT_TIMERFACTORY_H
#define MEMKIT_TIMERFACTORY_H

#include <zconf.h>
#include <iostream>
#include <thread>
#include <vector> /*for vector*/
/**
 * you should give me a function  pointer like this to run..
 */
typedef void (*timeFP)();
/**
 * use sleep and while to do the timer.
 */
class TimerFactory{
private:
protected:
   /**
    * this is the real worker
    * @param fp  the function need to be run
    * @param start_time  after
    * @param delay_time  loop
    * @param end_time   how many seconds to end
    */
    static void puppet(timeFP fp,long start_time,long delay_time,long end_time){
        long run_time_till_now=0;
        while(true){
            if(end_time!=-1&&run_time_till_now>=end_time){
                std::cout<<"Timer timeout~"<<std::endl;
                break;
            }
            sleep(start_time);
            run_time_till_now+=start_time;
            fp();
            sleep(delay_time);
            run_time_till_now+=delay_time;
        }
    }
public:
    /**
     * the default start time is begin,the delay is 60s
     * @param start
     * @param delay
     * @param end end=-1 means forever
     * @param fp the run function
     * @return
     */
    TimerFactory(timeFP fp ,long start=0,long delay=60,long end=-1){
        std::thread* timerThread=new std::thread(puppet,fp,start,delay,end);
        timerThread->detach();
    }
};

/**
 * another timer,you should extend this class,then implement the execute function
 * to do your logic
 */
class BaseTimer{
private:
    long start_time;
    long delay_time;
    long last_time;
protected:
     void puppet(){
        long run_time_till_now=0;
        while(true){
            if(this->last_time!=-1&&run_time_till_now>=last_time){
                std::cout<<"Timer timeout~"<<std::endl;
                break;
            }
            sleep(start_time);
            run_time_till_now+=start_time;
            this->execute();
            sleep(delay_time);
            run_time_till_now+=delay_time;
        }
    }

public:
    BaseTimer(long start,long delay,long last){
        this->start_time=start;
        this->delay_time=delay;
        this->last_time=last;
        std::thread Timer(&BaseTimer::puppet,this);
        Timer.detach();
    }
    /**
     * you should do the job on the function
     */
    virtual  void execute(){
        //todo
    }
};

#endif //MEMKIT_TIMERFACTORY_H
