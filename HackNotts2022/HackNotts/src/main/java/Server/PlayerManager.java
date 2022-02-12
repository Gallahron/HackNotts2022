package Server;

import java.util.HashMap;

public class PlayerManager {
    public static HashMap<Integer, Integer> playerLookup = new HashMap<Integer, Integer>();
    public static int currPlayer = 1;

    public static int AddPlayer(int machineID) {
        playerLookup.put(machineID, currPlayer);
        return currPlayer++;
    }
}
