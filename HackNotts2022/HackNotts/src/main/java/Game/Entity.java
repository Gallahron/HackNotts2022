package Game;

public class Entity {
    private int entityType;
    private int entityNumber;

    private float xPos;
    private float yPos;

    private float xSpeed;
    private float ySpeed;

    public Entity(float x, float y, float xSpeed, float ySpeed, int entityNumber, int entityType) {
        this.xPos = x;
        this.yPos = y;
        this.xSpeed = xSpeed;
        this.ySpeed = ySpeed;
        this.entityNumber = entityNumber;
        this.entityType = entityType;
    }

    public float getXPos() {
        return xPos;
    }

    public void setXPos(float xPos) {
        this.xPos = xPos;
    }

    public float getYPos() {
        return yPos;
    }

    public void setYPos(float yPos) {
        this.yPos = yPos;
    }

    public float getXSpeed() {
        return xSpeed;
    }

    public void setXSpeed(float xSpeed) {
        this.xSpeed = xSpeed;
    }

    public float getYSpeed() {
        return ySpeed;
    }

    public void setYSpeed(float ySpeed) {
        this.ySpeed = ySpeed;
    }

    public int getEntityType() {
        return entityType;
    }

    public void setEntityType(int entityType) {
        this.entityType = entityType;
    }

    public int getEntityNumber() {
        return entityNumber;
    }

    public void setEntityNumber(int entityNumber) {
        this.entityNumber = entityNumber;
    }
}
