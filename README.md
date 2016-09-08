MemKit (LTS)
===========================
No.1 what's this?
--------------------------
```
if you know about memcache,redis,you can understand the memkit.      
this is a key-value storage inmemory,and the engine is map of 
c++,and you should know about c++11,in a word,this is a key-value 
store engine    
and this memcached is multi-thread.you can cinfig the backlog to    
receive many connection from client,and the data is same to every   
client.and there are some features of this cache kit you should know:   
1.this cache is in-memory,i don't know if the 'dump'/'loadFromfile' 
works well.but i do not think you need to use the stupid function.    
2.the timer is very stupid,so after you set the new ttl for item,the put time   
will reset,and increase from now,the time reach the ttl time,but the timer still  
sleep,this case will delay the remove time.so,the memory's data is no so 'new'. 
3.you should offer a config file just like the project,but you can ignore it and    
let the memkit choose for you   
4.the recommend operators is:put,get,set,ttl,setc,rmappend,flush,flushAll,info    
and you should do not use the functions if possible:load.(in the beginning,i want to    
let you choose a dump file to rebuild the dataset in the memory,but,the formate of data   
is very import,if you do not ensure the formate of the dump file is right,please do not   
use this way to rebuild dataset)    
5.the memkit also offer a console runner,this is for server user    
6.you should choose the 'remote' command to set up the memkit on the server   

```

No.2 why to do this?
-------------------------
```
i like it.  
```

No.3 how to use it?
-------------------------
  i offer a java api for you here:<https://github.com/pandening/MemKit/blob/master/MemKitClient/src/net/hujian/memkit/client/JMemKitClient.java>     
  and the demo here:<https://github.com/pandening/MemKit/blob/master/MemKitClient/src/TestClient.java>      
  enjoy it!       
  the follow codes will tell you how to use the memkit.it's very easy to use.   
```
        os<<"\tload      [filePath]                                load cache from file,the old mem will be cleared."<<el;
        os<<"\tset       [store id][key][time to live(mills)]      set ttl for store@key"<<el;
        os<<"\tttl       [store id][key]                           get the ttl of store@key"<<el;
        os<<"\tget       [store id][key]                           get the value"<<el;
        os<<"\tdump      [true/false]                              dump to disk.(true means flush the cache)"<<el;
        os<<"\tappend    [store id][key][append value]             append a value"<<el;
        os<<"\tput       [store id][key][value]                    put a key-value"<<el;
        os<<"\tflush     [store]                                   flush the storage"<<el;
        os<<"\tflusha                                              flush all"<<el;
        os<<"\tinfo                                                show the size"<<el;
        os<<"\tsetc      [new capacity]                            re-set the capacity"<<el;
        os<<"\trm        [store id][key]                           delete the key-value"<<el;
        os<<"\texit                                                exit~"<<el;

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
1. 2016/9/7 set up the project.(too much bugs....)      
2. 2016/9/8 fix many bugs,include the timer.now the memkit can work~      
```

No.6 Developer
------------------------
  HuJian,E-mail:<1425124481@qq.com>  

No7. License
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
