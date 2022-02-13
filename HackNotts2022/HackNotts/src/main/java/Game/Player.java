package Game;

public class Player extends Entity {
    public final float MAX_SPEED = 5;
    final float JUMP_SPEED = 12;
    public int facing = 1;
    public int health = 3;
    int jumpCount = 2;

    public Player(int playerNo, float x, float y, float xSpeed, float ySpeed) {
        super(x, y, xSpeed, ySpeed, 1f, 1.5f, playerNo, 0, true, 3);
    }

    public void Jump() {
        if (grounded) jumpCount = 2;
        if (jumpCount > 0) {
            setYSpeed(JUMP_SPEED);
            setYPos(getYPos()+0.05f);
            NormaliseSpeed();
            jumpCount--;
        }
    }

    public void Hurt() {
        System.out.print("HURT!!!!!!!");
        setState(getState() - 1);
    }

    @Override
    public boolean zeroState() {
        setXPos(100);
        setYPos(100);
        return false;
    }

}
