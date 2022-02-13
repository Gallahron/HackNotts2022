package Game;

import Server.InputState;
import Server.InputStates;
import Server.PlayerManager;
import Server.Server;
import Server.NetworkPlayer;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.concurrent.TimeUnit;

public class GameController {
    ArrayList<Entity> entities = new ArrayList<Entity>();
    ArrayList<Player> players = new ArrayList<Player>();
    Server server;

    float deltaTime;
    long oldTime;

    public GameController(Server server) {
        this.server = server;
        entities.add(new Bullet(0,0,32,0));
        updateTime();
    }

    public void Run() {
        while (true) {
            try {
                updateTime();

                updatePlayerInputs();

                handlePhysics();

                outputData();
                //draw();

                TimeUnit.SECONDS.sleep(1);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    void updateTime() {
        Date date = new Date();
        long time = date.getTime();
        deltaTime = (time - oldTime) / 1000.0f;
        oldTime = time;
    }

    void updatePlayerInputs() {
        try {
            InputStates.available.acquire();
            for (int i = 1; i < PlayerManager.currPlayer; i++) {
                if (i > players.size()) {
                    Player player = new Player(i,0,0,0,32);
                    players.add(player);
                    entities.add(player);
                }
                InputState state = InputStates.GetState(i);
                Player player = players.get(i-1);
                player.setXSpeed(player.MAX_SPEED * (state.GetState("R") - state.GetState("L")));
            }
            InputStates.available.release();
        } catch (InterruptedException e) {}
    }

    void draw() {
        StringBuilder[] out = new StringBuilder[15];
        for (int i = 0; i < 15; i++) out[i] = new StringBuilder("                      ");

        for (Entity entity : entities) {
            char display = ' ';
            switch (entity.getEntityType()) {
                case (0) -> display = '0';
                case (1) -> display = '-';
            }
            out[(int)(entity.getYPos() / 32) % 15].setCharAt((int)(entity.getXPos() / 32) % 20, display);
        }
        System.out.println(deltaTime);
        System.out.println("-----------");
        for (StringBuilder i : out) {
            System.out.println(i);
        }
        System.out.println("------------");
    }

    void handlePhysics() {
        for (Entity entity : entities) {
            entity.move(deltaTime);
            //System.out.println(entity.getXPos());
        }
    }

    void outputData() {
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

        System.out.println(message);
        for (NetworkPlayer i : PlayerManager.playerLookup.values()) {
            server.SendMessage(message, i.getAddress(), i.getPort());
        }
    }

}
