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

#include "../MemKitUtils/MemKitUtils.h"


#define config_file "./conf/config.conf" /*the config file,do not change the file name by yourself*/
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
    /**
     * the reader.
     */
    std::ifstream reader;
    /**
     * the constructor is private
     * @return
     */
    MemKitConfig(){
        Configure=NULL;
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
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='i'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
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
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='p'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
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
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='c'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
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
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='r'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
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
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='t'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="3600";
        }
        if(dConfig){
            std::cout<<"get the ttl time:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the backlog
     * @return
     */
    String getBackLog(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='b'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="10";
        }
        if(dConfig){
            std::cout<<"get the backlog:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the slave name
     * @return
     */
    String getName(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='n'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line=getIP()+"@"+getPort();
        }
        if(dConfig){
            std::cout<<"get the name:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the manager ip
     * @return
     */
    String getManagerIP(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='m'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="127.0.0.1";
        }
        if(dConfig){
            std::cout<<"get the manager ip:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the manager port
     * @return
     */
    String getManagerPort(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='M'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="1234";
        }
        if(dConfig){
            std::cout<<"get the manager port:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }

    String getFlag(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='F'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="false";
        }
        if(dConfig){
            std::cout<<"get the flag:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * the default is do not
     * @return
     */
    String getAutoRebuildFlag(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='R'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="false";
        }
        if(dConfig){
            std::cout<<"get the auto rebuild flag:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * 1 minutes to dump..
     * @return
     */
    String getAutoDumpTime(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='D'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="60";
        }
        if(dConfig){
            std::cout<<"get the dump time:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
    /**
     * get the log file
     * @return
     */
    String getDumpFile(){
        reader.open(config_file);
        String line;
        while(getline(reader,line)){
            if(line[0]=='$'&&line[1]=='f'){
                line=trim(line);
                line=line.substr(line.find_first_of(':')+1);
                break;
            }
        }
        if(line.empty()){
            line="./conf/dump.log";
        }
        if(dConfig){
            std::cout<<"get the dump file:"<<line<<std::endl;
        }
        reader.close();
        return line;
    }
};


#endif //MEMKIT_CONFIGURE_H
