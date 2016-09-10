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
#ifndef MEMKIT_TEMPLATEOFTIMER_H
#define MEMKIT_TEMPLATEOFTIMER_H

#include "TimerFactory.h"

/**
 * a template for your own timer by extend the base timer
 */
class TimerTemplate:public BaseTimer{
private:
public:
    TimerTemplate(long start, long delay, long last) : BaseTimer(start, delay, last) {
    }
    /**
     * do your job~
     */
    void execute(){
        //todo
    }
};

#endif //MEMKIT_TEMPLATEOFTIMER_H
