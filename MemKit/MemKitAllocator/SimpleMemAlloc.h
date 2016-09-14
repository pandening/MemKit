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
#ifndef MEMKIT_SIMPLEMEMALLOC_H
#define MEMKIT_SIMPLEMEMALLOC_H

#include "MemoryPool.h"
/**
 * this is the simple memkit allocator
 */
template<class Type,class Alloc=hjstl_second_malloc>
class SimpleMemAlloc{
public:
    /**
     * get n object of Type.
     * @param n
     * @return
     */
    static Type* allocate(size_t n){
        return 0 == n ? 0 : (Type*)Alloc::hjstl_allocate(n*sizeof(Type));
    }
    /**
     * get an object
     * @return
     */
    static Type* allocate(){
        return (Type*)Alloc::hjstl_allocate(sizeof(Type));
    }
    /**
     * back.
     * @param ptr
     * @param n
     */
    static void deallocate(Type* ptr, size_t n){
        if (0 != n){
            Alloc::hjstl_deallocate(ptr, n*sizeof(Type));
        }
    }
    /**
     * back.
     * @param ptr
     */
    static void deallocate(Type* ptr){
        Alloc::hjstl_deallocate(ptr, sizeof(Type));
    }
};
#endif //MEMKIT_SIMPLEMEMALLOC_H
