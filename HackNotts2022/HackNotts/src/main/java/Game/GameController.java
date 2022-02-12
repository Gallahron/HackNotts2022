package Game;

import Server.InputState;
import Server.InputStates;
import Server.PlayerManager;
import Server.Server;
import Server.NetworkPlayer;

import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

public class GameController {
    ArrayList<Entity> entities = new ArrayList<Entity>();
    ArrayList<Player> players = new ArrayList<Player>();
    Server server;

    public GameController(Server server) {
        this.server = server;
        entities.add(new Bullet(0,0,1,0));
    }

    public void Run() {
        while (true) {
            try {
                InputStates.available.acquire();
                for (int i = 1; i < PlayerManager.currPlayer; i++) {
                    if (i > players.size()) {
                        Player player = new Player(i,0,0,0,0);
                        players.add(player);
                        entities.add(player);
                    }
                    InputState state = InputStates.GetState(i);
                    Player player = players.get(i-1);
                    player.setXSpeed(player.MAX_SPEED * (state.GetState("R") - state.GetState("L")));
                }
                InputStates.available.release();

                String message = "DATA[";
                for (Entity entity : entities) {
                    message += String.format("A(AT%d_AN%d_PX%f_PY%f_SX%f_SY%f)",
                            entity.getEntityType(),
                            entity.getEntityNumber(),
                            entity.getXPos(),
                            entity.getYPos(),
                            entity.getXSpeed(),
                            entity.getYSpeed()
                    );
                }
                message += "]";

                for (NetworkPlayer i : PlayerManager.playerLookup.values()) {
                    server.SendMessage(message, i.getAddress(), i.getPort());
                }

                TimeUnit.SECONDS.sleep(1);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
