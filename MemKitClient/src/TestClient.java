import net.hujian.memkit.client.JMemKitClient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Created by hujian on 16-9-7.
 */
public class TestClient {
    /**
     * just print it
     * @param out
     */
    private static void print(String out){
        System.out.println(out);
    }

    /**
     * the main
     * @param args
     */
    public static void main(String[] args){
        control();
        System.exit(0);

        /**
         * the response
         */
        String response=null;
        /**
         * get the client instance
         */
        JMemKitClient client=JMemKitClient.getInstance();
        /**
         * set the new cache
         */
        print("re-set the capacity");
        response=client.setC(100000);
        long st=System.currentTimeMillis();
        /**
         * put 100000 item to server
         */
        for(int i=0;i<100000;i++){
            String u=String.valueOf(i);
            client.put(u,u,u);
        }
        long et=System.currentTimeMillis();
        print("put 100000 items to mem kit,take "+(et-st)+" ms");
        /**
         * get the info
         */
        print(client.info());

        /**
         * get 0,0->should be 0
         * get 1,1->should be 1
         */
        print("get 0@0=>"+client.get("0","0"));
        print("get 1@1=>"+client.get("1","1"));

        /**
         * set the ttl for 0@0 ->99999
         */
        client.set("0","0","99999");
        /**
         * get the ttl of 0@0
         */
        print("0@0's ttl:"+client.ttl("0","0"));

        /**
         * please remember exit,or the server will always wait for you~
         */
        client.exit();
    }

    /**
     * test insert into mem kit,this is the data
     */
    private static class data{
        private String storeId=null;
        private String key=null;
        private String value=null;
        public void setStoreId(String id){
            this.storeId=id;
        }
        public String getStoreId(){
            return this.storeId;
        }
        public void setKey(String k){
            this.key=k;
        }
        public String getKey(){
            return this.key;
        }
        public void setValue(String v){
            this.value=v;
        }
        public String getValue(){
            return this.value;
        }
        @Override
        public String toString(){
            return "put "+this.storeId+" "+this.key+" "+this.value;
        }
    }

    /**
     * if you want to control the server,just use this function
     * this is a remote console.you can control the server by this
     * client.
     */
    static void control(){
        try {
            Socket socket=new Socket("127.0.0.1",6423);
            BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
            PrintWriter os=new PrintWriter(socket.getOutputStream());
            BufferedReader is=new BufferedReader(new InputStreamReader(socket.getInputStream()));
            /**
             * the default mem size just 1000,so,reset the capacity
             */
            os.println("setc 1000000");
            os.flush();
            /**
             * this is a bug.but it's useful for you~
             */
            is.readLine();
            long st=System.currentTimeMillis();
            data d=new data();
            for(int i=0;i<100000;i++){
                d.setStoreId(String.valueOf(i));
                d.setKey(String.valueOf(i));
                d.setValue(String.valueOf(i));
                os.println(d.toString());
                os.flush();
                /**
                 * you need always receive the response from server
                 */
                is.readLine();
            }
            long et=System.currentTimeMillis();
            print("Total take time "+(et-st)+" ms");
            String line;
            while(true){
                line=in.readLine();
                os.println(line);
                os.flush();
                print("client:" + line);
                print("server:" + is.readLine());
                if(line.equals("exit")){
                    break;
                }
            }
            os.close();
            is.close();
            in.close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
