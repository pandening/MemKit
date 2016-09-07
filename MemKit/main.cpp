#include "ConsoleRunner.h"
#include "RemoteRunner.h"
#include "RefreshMem.h"

typedef struct remote_ds{
    int file_ds;
    MemKit* memKit;
}remote_ds_t;


/**
 * handler
 * @param info
 */
void do_remote_job(remote_ds_t info) {
    bool loop=true;
    while (loop) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        int rcv_len = 0;
        while(rcv_len==0||rcv_len==-1){
            rcv_len=recv(info.file_ds, buffer, sizeof(buffer), 0);
        }
        /**
         * 1.get
         * 2.put
         * 3.set
         * 4.ttl
         * 5.setc
         * 6.dump
         * 7.append
         * 8.flush
         * 9.flusha
         * 10.info
         * 11.exit
         * 12.load
         */
        String line, store_id, key, value, cmd, file, ttl, cap, clear, newV, command, response;
        std::vector<String> splitVec;
        line = buffer;
        if(buffer[strlen(buffer)-1]=='\n') {
            line=line.substr(0,line.length()-1);
        }
        os<<"Receive Length:"<<rcv_len<<"@command["<<line<<"]"<<el;
        splitVec.clear();
        splitVec = MemKitUtils::split(line, " ");
        if (splitVec.empty()) {
            continue;
        }
        if (DEBUG) {
            std::vector<std::string>::iterator vit = splitVec.begin();
            for (; vit != splitVec.end(); vit++) {
                std::cout << "-------->[" << *vit << "]" << std::endl;
            }
        }
        command = splitVec[0];
        switch (command[0]) {
            case 'e': {
                os << "\tclose the file des connection,Bye~" << el;
                response="exit\n";
                int send_len=send(info.file_ds, response.c_str(), strlen(response.c_str()), 0);
                response="";
                close(info.file_ds);
                loop=false;
                break;
            }
            case 'l': {//load from file
                cmd = splitVec[0];
                if (cmd != "load" && cmd != "LOAD") {
                    response = "error";
                    if (DEBUG) {
                        os << "cmd:[" << cmd << "]" << el;
                    }
                    break;
                } else {
                    /**
                     * get the file
                     */
                    file = splitVec[1];
                    /**
                     * action
                     */
                    info.memKit->loadFromFile(file);
                    response = "ok";
                    if (DEBUG) {
                        os << "file:[" << file << "]" << el;
                    }
                    break;
                }
            }
                /**
                  * 1. set the ttl
                  * 2. set the capacity
                  */
            case 's': {
                cmd = splitVec[0];
                if (DEBUG) {
                    os << "cmd:[" << cmd << "]" << el;
                }
                if (cmd == "set") {
                    /**
                     * get the store id and the key and the ttl
                     */
                    store_id = splitVec[1];
                    key = splitVec[2];
                    ttl = splitVec[3];
                    if (DEBUG) {
                        os << "store_id[" << store_id << "] key[" << key << "] ttl[" << ttl << "]" << el;
                    }
                    /**
                     * action
                     */
                    info.memKit->set(store_id, key, atol(ttl.c_str()));
                    if(DEBUG){
                        os<<"SET TTL=>"<<atol(ttl.c_str())<<el;
                    }
                    response = "ok";
                    break;
                } else if (cmd == "setc" || cmd == "SETC") {
                    /**
                     * get the new capacity
                     */
                    cap = splitVec[1];
                    /**
                     * action
                     */
                    info.memKit->setCapacity(atol(cap.c_str()));
                    response = MemKitUtils::parseString(info.memKit->capacity());
                    break;
                } else {
                    response = "0";
                    break;
                }
            }
            case 't': {//get the ttl
                store_id = splitVec[1];
                key = splitVec[2];
                /**
                 * action
                 */
                response = MemKitUtils::parseString(info.memKit->getTTL(store_id, key));
                if (DEBUG) {
                    os<<"src ttl:"<<info.memKit->getTTL(store_id,key)<<el;
                    os << "store_id[" << store_id << "] key[" << key << "]" << el;
                }
                break;
            }
            case 'g': {//get a value
                store_id = splitVec[1];
                key = splitVec[2];
                /**
                 * action
                 */
                response = info.memKit->get(store_id, key);
                if (DEBUG) {
                    os << "store_id[" << store_id << "] key[" << key << "]" << el;
                }
                break;
            }
            case 'd': {//dump
                clear = splitVec[1];
                if (clear == "true") {
                    info.memKit->dump(true);
                } else {
                    info.memKit->dump(false);
                }
                response = "ok";
                if (DEBUG) {
                    os << "flag[" << clear << "]" << el;
                }
                break;
            }
            case 'a': {
                store_id = splitVec[1];
                key = splitVec[2];
                newV = splitVec[3];
                /**
                 * action
                 */
                info.memKit->append(store_id, key, newV);
                response = "ok";
                if (DEBUG) {
                    os << "store_id[" << store_id << "] key[" << key << "]" << "new value[" << newV << "]" << el;
                }
                break;
            }
            case 'p': {//put
                store_id = splitVec[1];
                key = splitVec[2];
                value = splitVec[3];
                /**
                 * action
                 */
                info.memKit->put(store_id, key, value);
                response = "ok";
                if (DEBUG) {
                    os << "store_id[" << store_id << "] key[" << key << "] value[" << value << "]" << el;
                }
                break;
            }
            case 'f': {
                cmd = splitVec[0];
                if (DEBUG) {
                    os << "cmd[" << cmd << "]" << el;
                }
                if (cmd == "flush") {//flush
                    /**
                     * get the flush store id
                     */
                    store_id = splitVec[1];
                    /**
                     * action
                     */
                    info.memKit->flushStore(store_id);
                    response = "ok";
                    if (DEBUG) {
                        os << "store_id[" << store_id << el;
                    }
                    break;
                } else {//flush all
                    info.memKit->flushAll();
                    response = "ok";
                    break;
                }
            }
            case 'i': {//show the size
                response = MemKitUtils::parseString(info.memKit->capacity()) +
                           "," + MemKitUtils::parseString(info.memKit->size());
                break;
            }
            default: {
                break;
            }
        }
        /**
         * send the response
         */
        response+="\n";
        int send_len=send(info.file_ds, response.c_str(), strlen(response.c_str()), 0);
        if(DEBUG){
            os << "The response is:[" << response << "]" <<"send length:"<<send_len<<el;
        }
        response="";
    }
}
/**
 * remote runner
 */
void run_RemoteRunner()
{
    RemoteRunner* runner=new RemoteRunner();
    int server_sock_fd;
    socklen_t  length;
    struct sockaddr_in client_sockaddr;
    server_sock_fd=runner->get_server_sock_fd();
    while(1) {
        os<<"i am accept...."<<el;
        length = sizeof(client_sockaddr);
        int connection_fd = accept(server_sock_fd,
                                   (struct sockaddr *) &client_sockaddr, &length);
        if (connection_fd < 0) {
            os << "error" << el;
            perror("accept error");
            exit(1);
        } else {
            remote_ds_t rdt;
            rdt.file_ds=connection_fd;
            rdt.memKit=runner->getMemKit();
            std::thread Thread(do_remote_job,rdt);
            Thread.detach();
        }
    }
}
/**
 * the fresh~
 * @param seconds
 */
void run_refreshMem(long seconds){
    /**
     * it's easy to do this.
     */
    RefreshMem* refreshMem=new RefreshMem(seconds);
}

/**
 * console runner
 */
void run_Console()
{
    ConsoleRunner* runner=new ConsoleRunner(1024);
    runner->run();
}

/**
 * the main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char**argv) {
    /**
     * first of all,set up the check
     */
    os<<"the time to refresh time is:";
    long fresh;
    is>>fresh;
    while(fresh<10){
        os<<"too small to check(fresh>10):";
        is>>fresh;
    }
    String cmd;
    while(1){
        os<<"\tlocal/remote/exit:";
        is>>cmd;
        if(cmd!="local"&&cmd!="remote"&&cmd!="exit"){
            continue;
        }
        if(cmd=="local"){
            std::thread console(run_Console);
            console.detach();
        }else if(cmd=="remote"){
            std::thread freshThread(run_refreshMem,fresh);
            freshThread.detach();
            run_RemoteRunner();
        }else{
            os<<"Bye"<<el;
            break;
        }
    }
    return 0;
}
