package Main;

public class Player extends Entity {
    private int playerNo;

    public Player(int playerNo, float x, float y, float xSpeed, float ySpeed) {
        super(x, y, xSpeed, ySpeed);

        this.playerNo = playerNo;
    }
}
