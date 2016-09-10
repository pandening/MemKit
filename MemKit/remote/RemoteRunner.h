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

#ifndef MEMKIT_REMOTERUNNER_H
#define MEMKIT_REMOTERUNNER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <thread> /*so,c++ 11 is needed*/

/**
 * for easy,i just define the port and ip in this file.
 * there will be a config file in the futher.
 */

#define BUFFER_SIZE 1024

#define is  std::cin
#define os  std::cout
#define el  std::endl

#include "../core/MemKit.h"
/**
 * the remote runner
 */
class RemoteRunner{
private:
    /**
     * the server sock file des
     */
    int server_sock_fd;
    /**
     * the sock addr
     */
    struct sockaddr_in server_sockaddr;
    /**
     * the mem kit
     */
    MemKit* RemotememKit;
    /**
     * the config
     */
    MemKitConfig* config;
protected:
    /**
     * get the file des
     */
    int get_sock_fd(){
        this->server_sock_fd=socket(AF_INET,SOCK_STREAM,0);
        if(this->server_sock_fd==-1){
            os<<"can not get a sock file des."<<el;
            exit(-1);
        }
        return this->server_sock_fd;
    }
    /**
     * set up the sock addr
     */
    void set_up_sock_addr(){
        this->server_sockaddr.sin_family=AF_INET;
        this->server_sockaddr.sin_port=htons(atoi(config->getPort().c_str()));
        this->server_sockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    }
    /**
     * server bind
     */
    void Bind(){
        if(bind(server_sock_fd,(struct sockaddr*)&server_sockaddr,sizeof(server_sockaddr))==-1){
            os<<"bind error"<<el;
            exit(-1);
        }
    }
    /**
     * listen
     */
    void Listen(){
        if(listen(server_sock_fd,atol(config->getBackLog().c_str()))==-1){
            os<<"listener error"<<el;
            exit(-1);
        }
    }
public:
    /**
     * the constructor
     * @return
     */
    RemoteRunner(){
        config=MemKitConfig::getConfigure();
        this->get_sock_fd();
        this->set_up_sock_addr();
        this->Bind();
        this->Listen();
        this->RemotememKit=MemKit::getInstance(atol(config->getCapacity().c_str()));
    }
    /**
     * get the mem kit
     * @return
     */
    MemKit* getMemKit(){
        if(this->RemotememKit==NULL){
            this->RemotememKit=MemKit::getInstance(atol(config->getCapacity().c_str()));
        }
        return this->RemotememKit;
    }

    /**
     * get the sock
     * @return
     */
    int get_server_sock_fd(){
        return this->server_sock_fd;
    }
    /**
     * get the server sockaddr
     * @return
     */
    struct sockaddr_in get_sockaddr(){
        return this->server_sockaddr;
    }
};

#endif //MEMKIT_REMOTERUNNER_H
