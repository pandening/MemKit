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

#ifndef MEMKIT_CONFIGURE_H
#define MEMKIT_CONFIGURE_H

#include "MemKitUtils.h"


#define config_file "./config.conf" /*the config file,do not change the file name by yourself*/
#define dConfig 0
typedef std::string String;

/**
 * the only instance of this class
 */
static class MemKitConfig* Configure;

/**
 * you can get the config information by this class
 * NOTICE,THE CONFIGURE FILE IS NEEDED,BUT IF YOU WANT TO RUN AS DEFAULT
 * JUST IGNORE THE CONFIGURE FILE.THE CLASS WILL GET A DEFAULT VALUE FOR
 * EACH CONFIG.
 */
class MemKitConfig{
private:
    String ip;
    String port;
    String capacity;
    String rTime;
    /**
     * the reader.
     */
    std::ifstream reader;
    /**
     * the constructor is private
     * @return
     */
    MemKitConfig(){
        ip="";
        port="";
        capacity="";
        rTime="";
        Configure=NULL;
        /**
         * open the config file
         */
        reader.open(config_file);
    }

    /**
      * like trim
      * @param source
      * @return
     */
    String trim(String source){
        if(source.empty()){
            return source;
        }
        source.erase(0,source.find_first_not_of(" "));
        source.erase(source.find_last_not_of(" ")+1);
        return source;
    }

public:
    /**
     * if you want to get the config
     * @return
     */
    static MemKitConfig* getConfigure(){
        if(Configure==NULL){
            Configure=new MemKitConfig;
        }
        return Configure;
    }
    /**
     * get the ip
     * @return
     */
    String getIP(){
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='i'){
                line=trim(line);
                line=line.substr(line.find_first_of(':'));
                break;
            }
        }
        if(line.empty()){
            line="127.0.0.1";
        }
        if(dConfig){
            std::cout<<"get the ip:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the port
     * @return
     */
    String getPort(){
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='p'){
                line=trim(line);
                line=line.substr(line.find_first_of(':'));
                break;
            }
        }
        if(line.empty()){
            line="6423";
        }
        if(dConfig){
            std::cout<<"get the port:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the capacity
     * @return
     */
    String getCapacity(){
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='c'){
                line=trim(line);
                line=line.substr(line.find_first_of(':'));
                break;
            }
        }
        if(line.empty()){
            line="1000";
        }
        if(dConfig){
            std::cout<<"get the capacity:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the refresh time
     * @return
     */
    String getRTime(){
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='r'){
                line=trim(line);
                line=line.substr(line.find_first_of(':'));
                break;
            }
        }
        if(line.empty()){
            line="60";
        }
        if(dConfig){
            std::cout<<"get the refresh time:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the default ttl
     * @return
     */
    String getTTL(){
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='t'){
                line=trim(line);
                line=line.substr(line.find_first_of(':'));
                break;
            }
        }
        if(line.empty()){
            line="3600";
        }
        if(dConfig){
            std::cout<<"get the refresh time:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the backlog
     * @return
     */
    String getBackLog(){
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='b'){
                line=trim(line);
                line=line.substr(line.find_first_of(':'));
                break;
            }
        }
        if(line.empty()){
            line="10";
        }
        if(dConfig){
            std::cout<<"get the refresh time:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
};


#endif //MEMKIT_CONFIGURE_H
