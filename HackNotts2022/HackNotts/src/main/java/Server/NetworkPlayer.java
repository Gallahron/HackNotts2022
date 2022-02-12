package Server;

import java.net.InetAddress;

public class NetworkPlayer {
    private InetAddress address;
    private int port;
    private int playerNumber;

    public NetworkPlayer(int playerNumber, InetAddress address, int port) {
        this.address = address;
        this.port = port;
        this.playerNumber = playerNumber;
    }

    public InetAddress getAddress() {
        return address;
    }

    public int getPort() {
        return port;
    }

    public int getPlayerNumber() {
        return playerNumber;
    }
}
