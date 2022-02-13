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
    Arena map;

    float gravity = -15;

    float deltaTime;
    long oldTime;

    public GameController(Server server) {
        this.server = server;
        //entities.add(new Bullet(0.04f,7,3,0, entities));
        map = new Arena();
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

                TimeUnit.MILLISECONDS.sleep(5);
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
                    Player player = new Player(i, (2 + 3 * (i-1)) % map.maxArenaX, 2f, 3, 0);
                    players.add(player);
                    entities.add(player);
                }
                InputState state = InputStates.GetState(i);
                Player player = players.get(i-1);
                float speed = player.MAX_SPEED * (state.GetState("R") - state.GetState("L"));
                player.setXSpeed(speed);
                player.facing = speed != 0 ? (int)(speed / Math.abs(speed)) : player.facing;

                if (state.GetState("J") == 1 && !state.jumpToggle) {
                    player.Jump();
                    state.jumpToggle = true;
                } else if (state.GetState("J") == 0 && state.jumpToggle) state.jumpToggle = false;
                if (state.GetState("S") == 1 && !state.shootToggle) {
                    Bullet bullet = new Bullet(
                            player.getXPos() + player.facing * 1.5f,
                            player.getYPos() + player.getYRad() / 2,
                            player.facing * 12,
                            (float) (-0.1 + Math.random() * (0.1 - -0.1)),
                            entities
                    );
                    state.shootToggle = true;
                    entities.add(bullet);
                } else if (state.GetState("S") == 0 && state.shootToggle) state.shootToggle = false;
            }
            InputStates.available.release();
        } catch (InterruptedException e) {}
    }

    void draw() {
        StringBuilder[] out = new StringBuilder[15];
        for (int i = 0; i < 15; i++) out[i] = new StringBuilder(map.mapData[i]);

        for (Entity entity : entities) {
            char display = ' ';
            switch (entity.getEntityType()) {
                case (0) -> display = '0';
                case (1) -> display = '-';
            }
            out[(int)(map.maxArenaY - (entity.getYPos()) % map.maxArenaY)].setCharAt((int)(entity.getXPos()) % map.maxArenaX, display);
        }
        System.out.println(deltaTime);
        System.out.println("-----------");
        for (StringBuilder i : out) {
            System.out.println(i);
        }
        System.out.println("------------");
    }

    void handlePhysics() {
        ArrayList<Entity> removal = new ArrayList<Entity>();

        for (Entity entity : entities) {
            float prevX = entity.getXPos();
            float prevY = entity.getYPos();
            boolean collided = false;

            if (entity.useGravity) {
                int velocDir = (int)Math.signum(entity.getYSpeed());
                int gravDir = (int)Math.signum(gravity);
                entity.modYSpeed(gravity * deltaTime * ((velocDir == gravDir) ? 1.5f : 1));
            }

            entity.move(deltaTime);
            if (entity.getXPos() + entity.getXRad() < 0) entity.setXPos(map.maxArenaX);
            else if (entity.getXPos() > map.maxArenaX) entity.setXPos(-entity.getXRad());

            entity.grounded = false;
            for (Block other:map.blocks) {
                int dir = entity.isCollided(other);
                if (dir % 2 == 1) {
                    entity.setYPos(prevY);
                    entity.setYSpeed(0);
                    if (dir == 1) entity.grounded = true;
                } else if (dir % 2 == 0) {
                    entity.setXPos((prevX));
                    entity.setXSpeed(0);
                }
                if (dir != -1) {
                    if (entity.OnCollision(other)) removal.add(entity);
                    break;
                }
            }

            for (Entity other:entities) {
                if (other == entity) continue;

                int dir = entity.isCollided(other);
                if (dir % 2 == 1) {
                    entity.setYPos(prevY);
                    entity.setYSpeed(0);
                    if (dir == 1) entity.grounded = true;
                } else if (dir % 2 == 0) {
                    entity.setXPos((prevX));
                    entity.setXSpeed(0);
                }
                if (dir != -1) {
                    if (entity.OnCollision(other)) removal.add(entity);
                    break;
                }
            }
        }
        for (Entity i : removal) {
            entities.remove(i);
        }
    }

    void outputData() {
        String message = "DATA(";
        for (Entity entity : entities) {
                message += String.format("A(AT%d_AN%d_AX%d_PX%f_PY%f_SX%f_SY%f)",
                        entity.getEntityType(),
                        entity.getEntityNumber(),
                        entity.getState(),
                        entity.getXPos(),
                        entity.getYPos(),
                        entity.getXSpeed(),
                        entity.getYSpeed()
                );
            if (entity.getState() == 0) entity.zeroState();

        }
        message += ")";

        //System.out.println(message);
        for (NetworkPlayer i : PlayerManager.playerLookup.values()) {
            server.SendMessage(message, i.getAddress(), i.getPort());
        }
    }

}
