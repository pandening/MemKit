//
// Created by hujian on 16-9-7.
//

#ifndef MEMKIT_REFRESHMEM_H
#define MEMKIT_REFRESHMEM_H

#include "MemKit.h"

#define os std::cout
#define is std::cin
#define el std::endl
/**
 * this class will check the mem,then remove the item (item's ttl<0)
 */
class RefreshMem{
private:
    /**
     * the fresh time
     */
    long time_to_refresh;
    /**
     * the mem,this ok because if the instance is not null,the new capacity is
     * not work out~
     */
    MemKit* Mem;
    std::map<String,std::map<String,ttl_t>> ttl_Map;
    /**
     * the puppet of this class
     */
    void puppet(){
        /**
         * forever
         */
        while(true){
            ttl_Map=Mem->getTTLStorage();
            os<<"i am timer,i am running..."<<el;
            long del=0;
            sleep(this->time_to_refresh);
            os<<"total size to scan=>"<<ttl_Map.size()<<el;
            if(ttl_Map.size()==0){
                continue;
            }
            std::map<String,std::map<String,ttl_t>>::iterator sit=this->ttl_Map.begin();
            std::map<String,ttl_t>::iterator kit;
            for(;sit!=ttl_Map.end();sit++){
                for(kit=(*sit).second.begin();kit!=(*sit).second.end();kit++){
                    (*kit).second.leave=time((time_t*)NULL)-(*kit).second.putTime;
                }
            }
            os<<"Total delete of time out items:"<<del<<el;
        }
    }
public:
    /**
     * the constructor
     * @param ftime
     * @return
     */
    RefreshMem(long ftime){
        Mem=MemKit::getInstance(1234);
        this->time_to_refresh=ftime;
        this->puppet();
    }
};
#endif //MEMKIT_REFRESHMEM_H
