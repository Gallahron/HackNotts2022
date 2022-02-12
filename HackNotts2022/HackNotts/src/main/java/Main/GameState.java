package Main;

import java.util.ArrayList;

public class GameState {
    private ArrayList<Bullet> bullets;
    private ArrayList<Player> players;

    public void AddPlayer(int playerNo, float x, float y, float xSpeed, float ySpeed) {
        Player player = new Player(playerNo, x, y, xSpeed, ySpeed);

        players.add(player);
    }

    public void AddBullet(float x, float y, float xSpeed, float ySpeed) {
        Bullet bullet = new Bullet(x, y, xSpeed, ySpeed);

        bullets.add(bullet);
    }

}
