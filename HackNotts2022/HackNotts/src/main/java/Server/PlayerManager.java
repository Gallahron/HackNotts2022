package Server;

import java.net.InetAddress;
import java.util.HashMap;

public class PlayerManager {
    public static HashMap<Integer, NetworkPlayer> playerLookup = new HashMap<Integer, NetworkPlayer>();
    public static int currPlayer = 1;

    public static int AddPlayer(int machineID, InetAddress address, int port) {
        playerLookup.put(machineID, new NetworkPlayer(currPlayer, address, port));
        return currPlayer++;
    }
}
