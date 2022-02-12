package Game;

import Server.InputState;
import Server.InputStates;
import Server.PlayerManager;

import java.util.ArrayList;

public class GameController {
    ArrayList<Player> players = new ArrayList<Player>();


    public GameController() {

    }
    public void Run() {
        while (true) {
            try {
                InputStates.available.acquire();
                for (int i = 1; i < PlayerManager.currPlayer; i++) {
                    if (i > players.size()) players.add(new Player(i,0,0,0,0));
                    InputState state = InputStates.GetState(0);
                    Player player = players.get(i-1);
                    player.setXSpeed(player.MAX_SPEED * (state.GetState("R") - state.GetState("L")));
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
