package Game;

public class Player extends Entity {
    public static float MAX_SPEED;

    public Player(int playerNo, float x, float y, float xSpeed, float ySpeed) {
        super(x, y, xSpeed, ySpeed, playerNo, 0);
    }
}
