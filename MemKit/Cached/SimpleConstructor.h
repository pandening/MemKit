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
#ifndef MEMKIT_SIMPLECONSTRUCTOR_H
#define MEMKIT_SIMPLECONSTRUCTOR_H

#include <new>

/**
 * you want to destroy an pointer
 * @param pointer
 */
template <class Type>
inline void destroy(Type* pointer){
    pointer->~Type();
}
/**
 * you need  the constructor to get an object
 * @param p
 * @param value
 */
template <class T1,class T2>
inline void constructor(T1* p,T2&value){
    new (p)T1(value);
};

#endif //MEMKIT_SIMPLECONSTRUCTOR_H
