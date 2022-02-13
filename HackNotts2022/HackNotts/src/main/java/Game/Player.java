package Game;

public class Player extends Entity {
    public final float MAX_SPEED = 32;

    public Player(int playerNo, float x, float y, float xSpeed, float ySpeed) {
        super(x, y, xSpeed, ySpeed, 1, 1.5f, playerNo, 0);
    }
}
