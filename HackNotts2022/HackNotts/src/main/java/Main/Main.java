package Main;

import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Main {
    public static GameState state;
    public static Server server;

    public static HashMap<Integer, Integer> playerLookup = new HashMap<Integer, Integer>();
    public static int currPlayer = 1;

    public static void main(String args[]){
        server = new Server(6969);
        server.OpenServer();
    }

    public static int AddPlayer(int machineID) {
        playerLookup.put(machineID, currPlayer);
        return currPlayer++;
    }

}
