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

#ifndef MEMKIT_COMPUTERMEM_H
#define MEMKIT_COMPUTERMEM_H

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>

/**
 * get the computer memory,for setC
 * NOTICE:YOU SHOULD DO NOT RE-CALL THIS CLASS.BECAUSE
 * THIS CLASS WILL READ FILE TO GET THE INFORMATION,THEN
 * THIS CLASS WILL ASSIGN THE MEM SIZE TO THE MEMKIT.
 * SO,YOU CAN JUST CALL THE MEMKIT TO GET THE MEM INFO
 */
class ComputerMem{
private:
    std::string totalMemSize;
    std::string usedMemSize;
    std::string freeMemSize;
    /**
     * the mem
     */
    MemKit* Mem;
protected:
    void puppet(){
        system("free -m >>mem.txt");
        std::ifstream reader("mem.txt");
        std::string line,tmp;
        while(getline(reader,line)){
            line=MemKitUtils::trim(line);
            if(line[0]=='M'){
                line=line.substr(4);
                line=line.substr(line.find_first_not_of(" "));
                tmp=line.substr(line.find_first_of(" "));
                line=line.substr(0,line.find_first_of(" "));
                this->totalMemSize=line;
                line=tmp;
                line=line.substr(line.find_first_not_of(" "));
                tmp=line;
                line=line.substr(0,line.find_first_of(" "));
                this->usedMemSize=line;
                line=tmp.substr(tmp.find_first_of(" "));
                line=line.substr(line.find_first_not_of(" "));
                line=line.substr(0,line.find_first_of(" "));
                this->freeMemSize=line;
                /**
                 * set to..
                 */
                this->Mem->setTotalMem(atoi(this->totalMemSize.c_str()));
                this->Mem->setUsedMem(atoi(this->usedMemSize.c_str()));
                this->Mem->setFreeMem(atoi(this->freeMemSize.c_str()));
                break;
            }
        }
        reader.close();
        system("rm mem.txt");
    }
public:
    ComputerMem(){
        Mem=MemKit::getInstance(0);
        this->puppet();
    }
    /**
     * get the mem.
     * @return
     */
    int getTotalMemSize(){
        return atoi(this->totalMemSize.c_str());
    }
    /**
     * get free mem
     * @return
     */
    int getFreeMemSize(){
        return atoi(this->freeMemSize.c_str());
    }
    /**
     * get used mem
     * @return
     */
    int getUsedMemSize(){
        return atoi(this->usedMemSize.c_str());
    }
};
#endif //MEMKIT_COMPUTERMEM_H
