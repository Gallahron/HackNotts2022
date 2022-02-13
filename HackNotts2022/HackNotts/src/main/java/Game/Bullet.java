package Game;

import java.util.ArrayList;

public class Bullet extends Entity {
    ArrayList entities;
    public Bullet(float x, float y, float xSpeed, float ySpeed, ArrayList<Entity> entities) {
        super(x, y, xSpeed, ySpeed, 0.3f, 0.3f, 0, 1, false, 15000);
        this.entities = entities;
    }

    @Override
    public boolean OnCollision(Entity other) {
        super.OnCollision(other);

        if (other.getClass() == Player.class) {
            ((Player) other).Hurt();
        }

        return true;
    }

    @Override
    public void move(float deltaTime) {
        super.move(deltaTime);
        setState(getState() - (int)(deltaTime * 1000));
    }

    @Override
    public boolean zeroState() {return true;}
}
