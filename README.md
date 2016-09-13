MemKit (LTS)
===========================
No.1 what's this?
--------------------------
```
if you know about memcache,redis etc.,you can understand the memkit.      
this is a key-value storage in memory like this:[store_id][key][value]    
the store_id just like the database of mysql.   
and this memcached is multi-thread.you can config the backlog to    
receive many connection from client,and the data is same to every   
client.and there are some features of this cache kit you should know:   
(1).this cache is in-memory,i don't know if the 'dump'/'loadFromfile' 
works well.but i do not think you need to use the stupid function.    
(2).the timer is very stupid,so after you set the new ttl for item,the put time   
will reset,and increase from now,the time reach the ttl time,but the timer still  
sleep,this case will delay the remove time.so,the memory's data is no so 'new'. 
(3).you should offer a config file just like the project,but you can ignore it and    
let the memkit choose for you   
(4).the recommend operators is:put,get,set,ttl,setc,rmappend,flush,flushAll,info    
and you should do not use the functions if possible:load.(in the beginning,i want to    
let you choose a dump file to rebuild the dataset in the memory,but,the formate of data   
is very import,if you do not ensure the formate of the dump file is right,please do not   
use this way to rebuild dataset)    
(5).the memkit also offer a console runner,this is for server user    
(6).you should choose the 'remote' command to set up the memkit on the server   
(7).The memcached is for Linux,not windows or other platform.   
(8).this memkit based on c++11.so,you should ensure your c++ colpiler support c++11 features    

```

No.2 why to do this?
-------------------------
```
i like it.  
```

No.3 how to use it?
-------------------------
######I offer a java api for you here:<https://github.com/pandening/MemKit/blob/master/MemKitClient/src/net/hujian/memkit/client/JMemKitClient.java>     
######And the demo here:<https://github.com/pandening/MemKit/blob/master/MemKitClient/src/TestClient.java>      
  Enjoy yourself!       
  the follow codes will tell you how to use the memkit.it's very easy to use.   
```
        os<<"\tload      [filePath]                                load cache from file,the old mem will be cleared."<<el;
        os<<"\tset       [store id][key][time to live(mills)]      set ttl for store@key"<<el;
        os<<"\tttl       [store id][key]                           get the ttl of store@key"<<el;
        os<<"\tget       [store id][key]                           get the value"<<el;
        os<<"\tdump      [file][true/false]                        dump to disk.(true means flush the cache)"<<el;
        os<<"\tappend    [store id][key][append value]             append a value"<<el;
        os<<"\tput       [store id][key][value]                    put a key-value"<<el;
        os<<"\tps        [store_id,key,value]list                  put a list"<<el;
        os<<"\tks        [store_id->default is null]               get the keys list"<<el;
        os<<"\tss                                                  get the stores list"<<el;
        os<<"\tflush     [store]                                   flush the storage"<<el;
        os<<"\tflusha                                              flush all"<<el;
        os<<"\tinfo                                                show the size"<<el;
        os<<"\tsetc      [new capacity]                            re-set the capacity"<<el;
        os<<"\trm        [store id][key]                           delete the key-value"<<el;
        os<<"\texist     [store id][key]                           judge if the id@key exist"<<el;
        os<<"\texistid   [store id]                                just check the store id"<<el;
        os<<"\texit                                                exit~"<<el;
        os<<"\thh                                                  alive check"<<el;
        os<<"\trs        [old store id][new store id]              rename a store id"<<el;
        os<<"\trk        [store id][old key][new key]              rename a key"<<el;
        os<<"\tre        [store id][key][replace to]               replace the value"<<el;
        os<<"\trd                                                  get an random key"<<el;
        os<<"\tmem                                                 get the mem info.<total,used,free>"<<el;

```
No.4 LTS 
--------------------------
```
this project is a LTS project, i will update the information after new  
functions are builded up.     
```
No.5 update information
-------------------------
```
1. 2016/9/7   set up the project.(too much bugs....)      
2. 2016/9/8   fix many bugs,include the timer.now the memkit can work~      
3. 2016/9/9   build up the memkit cluster version,parallel LTS with this project~
4. 2016/9/10  
            (1). re-organize the files,and set up the auto-dump engine,you can rebuild the memkit now~  
            (2). add new timer,let the timer to control the memkit's securty  
            (3). show the run interface for you~
5. 2016/9/11 
            (1). create the .sh file,you can use .sh file to run the memkit
            (2). more apis,such as 'ps','ss','ks',etc,you can check the consoleRunner.h file to get details 
            (3). new timer and thread factory,more fixable and stable.  
            (4). you get replace a key's value to another now,but you should offer the new value,not another store@value
            (5). you can rename a storage now,return true/false to show the result
            (6). you can rename a key's name,return true/false
            (7). add some new files,but now they are stupid.
6. 2016/9/13 
            (1). fix the load bugs,now the load function works well.
            (2). remove some useless codes.
            (3). move the .sh file to bin/
            (4). create a thread pool,but not work now.
            (5). the memkit can get the system's memory information now,then the manage can set capacity according to this info.
```

No.6 Developer
------------------------
###HuJian,E-mail:<1425124481@qq.com>  

No.7 Platform and Develope Tool
--------------------------------
```
Platform:Linux
Develop tool:Ubuntu 14.0.4 /64bits Clion 2.1,Ieda +jdk 8
```

No.8 License
------------------------
```
Copyright 2016 HuJian

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

No.9 Links
--------------------------------
###Another project based on this project:<https://github.com/pandening/MemKit-Cluster>    

No.10 Like?
--------------------------------
![image](https://github.com/pandening/images/blob/master/newStart.png)
![image](https://github.com/pandening/images/blob/master/op.png)
