package net.hujian.memkit.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Created by hujian on 16-9-7.
 * this is the java-mem kit client
 */
public class JMemKitClient {
    /**
     * this is the only instance of this class
     */
    private static JMemKitClient _instance=null;
    /**
     * the sock
     */
    Socket socket=null;
    /**
     * the writer
     */
    PrintWriter writer=null;
    /**
     * the reader
     */
    BufferedReader reader=null;

    /**
     * the constructor,it's private
     */
    private JMemKitClient(){
        /**
         * get the sock
         */
        try {
            socket=new Socket("127.0.0.1",6423);
        } catch (IOException e) {
            e.printStackTrace();
        }
        /**
         * get the is and os
         */
        try {
            writer=new PrintWriter(socket.getOutputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            reader=new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * get the only instance
     * @return
     */
    public static JMemKitClient getInstance(){
        if(_instance==null){
            _instance=new JMemKitClient();
        }
        return _instance;
    }

    /**
     * send a msg
     * @param msg
     */
    private void send(String msg){
        writer.println(msg);
        writer.flush();
    }

    /**
     * when you need to receive from mem kit server
     * @return
     */
    private String recv(){
        try {
            return reader.readLine();
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * load,you should offer a right path.
     * @param filepath
     * @return
     */
    public String load(String filepath){
        this.send("load "+filepath);
        return this.recv();
    }

    /**
     * set the ttl for the store_id@key
     * @param store_id
     * @param key
     * @param ttl
     * @return
     */
    public String set(String store_id,String key,String ttl){
        this.send("set " + store_id + " " + key + " " + ttl);
        return this.recv();
    }

    /**
     * get the store_id@key's ttl
     * @param store_id
     * @param key
     * @return
     */
    public String ttl(String store_id,String key){
        this.send("ttl "+store_id+" "+key);
        return this.recv();
    }

    /**
     * get an value from server
     * @param store_id
     * @param key
     * @return
     */
    public String get(String store_id,String key){
        this.send("get "+store_id+" "+key);
        return this.recv();
    }

    /**
     * dump to file
     * @param flag true means you want to clear the old mem
     * @return
     */
    public String dump(String flag){
        this.send("dump "+flag);
        return this.recv();
    }

    /**
     * append an value to the key
     * @param store_id
     * @param key
     * @param append_value
     * @return
     */
    public String append(String store_id,String key,String append_value){
        this.send("append "+store_id+" "+key+" "+append_value);
        return this.recv();
    }

    /**
     * put a key-value to mem
     * @param store_id
     * @param key
     * @param value
     * @return
     */
    public String put(String store_id,String key,String value){
        this.send("put "+store_id+" "+key+" "+value);
        return this.recv();
    }

    /**
     * flush the store
     * @param store_id
     * @return
     */
    public String flush(String store_id){
        this.send("flush "+store_id);
        return this.recv();
    }

    /**
     * just clear the cache
     * @return
     */
    public String flushAll(){
        this.send("flusha");
        return this.recv();
    }

    /**
     * get simple info from server
     * @return capacity,size
     */
    public String info(){
        this.send("info");
        return this.recv();
    }

    /**
     * set new capacity
     * NOTICE:YOU WILL LOST DATA IF THE NEW CAPACITY<OLD CAPACITY
     * @param capacity
     * @return
     */
    public String setC(long capacity){
        this.send("setc "+capacity);
        return this.recv();
    }

    /**
     * delete a key-value
     * @param store_id
     * @param key
     * @return
     */
    public  String delete(String store_id,String key){
        this.send("rm "+store_id+" "+key);
        return this.recv();
    }

    /**
     * just close the connect from server
     * @return
     */
    public String exit(){
        this.send("exit");
        String rt=this.recv();
        this.writer.close();
        try {
            this.reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            this.socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return rt;
    }
}
