package Main;

import Game.GameState;
import Server.Server;
import Server.GetIP;

import java.util.*;

public class Main {
    public static GameState state;
    public static Server server;

    public static void main(String args[]){
        for (String i : GetIP.getLocalHostAddresses()) System.out.println(i);

        server = new Server(6969);
        server.OpenServer();
    }
}
