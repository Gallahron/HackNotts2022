package Main;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Main {
    public static GameState state;
    public static Server server;

    public static HashMap<Integer, Integer> playerLookup = new HashMap<Integer, Integer>();
    public static int currPlayer = 1;

    public static void main(String args[]){
        try {
            URL whatismyip = new URL("http://checkip.amazonaws.com");

            BufferedReader in = null;
            in = new BufferedReader(new InputStreamReader(
                    whatismyip.openStream()));

            String ip = in.readLine(); //you get the IP as a String
            System.out.println(ip);
        } catch (IOException e) {
            e.printStackTrace();
        }

        server = new Server(6969);
        server.OpenServer();
    }

    public static int AddPlayer(int machineID) {
        playerLookup.put(machineID, currPlayer);
        return currPlayer++;
    }

}
