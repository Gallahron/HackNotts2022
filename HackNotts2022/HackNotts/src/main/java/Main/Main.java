package Main;

import java.io.IOException;
import java.net.*;

public class Main {
    public static void main(String args[]){
        DatagramSocket aSocket = null;
        try{System.out.println(InetAddress.getLocalHost());} catch (UnknownHostException e) {
            e.printStackTrace();
        }
        try{
            aSocket = new DatagramSocket(6969);
            byte[] buffer = new byte[1000];
            while(true){
                DatagramPacket request = new DatagramPacket(buffer, buffer.length);
                aSocket.receive(request);
                System.out.println("Request Recieved: " + request.getData().toString());
                String replyText = "This is the reply!";
                DatagramPacket reply = new DatagramPacket(replyText.getBytes(),
                        replyText.length(), request.getAddress(), request.getPort());
                aSocket.send(reply);
            }
        } catch (SocketException e){System.out.println("Socket: " + e.getMessage());
        } catch (IOException e) {System.out.println("IO: " + e.getMessage());
        } finally {if(aSocket != null) aSocket.close();}
    }
}
