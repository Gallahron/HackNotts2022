package Main;

import Game.GameController;
import Server.Server;
import Server.GetIP;

public class Main {
    public static Server server;
    public static GameController gameController;

    public static void main(String args[]){
        for (String i : GetIP.getLocalHostAddresses()) System.out.println(i);

        server = new Server(6969);

        Thread serverThread = new Thread() {
            public void run() {
                server.OpenServer();
            }
        };
        serverThread.start();

        gameController = new GameController(server);

        Thread gameThread = new Thread() {
            public void run() {
                gameController.Run();
            }
        };
        gameThread.start();
    }
}
