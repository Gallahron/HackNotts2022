package Game;

import java.util.PropertyResourceBundle;

public class Entity {
    private int entityType;
    private int entityNumber;

    private float xPos;
    private float yPos;

    private float xSpeed;
    private float ySpeed;

    private float xRad;
    private float yRad;

    public Entity(float x, float y, float xSpeed, float ySpeed, float xRad, float yRad, int entityNumber, int entityType) {
        this.xPos = x;
        this.yPos = y;
        this.xSpeed = xSpeed;
        this.ySpeed = ySpeed;
        this.xRad = xRad;
        this.yRad = yRad;
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

    public float getXRad() {
        return xRad;
    }

    public void setXRad(float xRad) {
        this.xRad = xRad;
    }

    public float getYRad() {
        return yRad;
    }

    public void setYRad(float yRad) {
        this.yRad = yRad;
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

    public void move(float time) {
        this.xPos += xSpeed * time;
        this.yPos += ySpeed * time;
    }

    public void stop() {
        this.xSpeed = 0;
        this.ySpeed = 0;
    }

    public int isCollided(Entity other) {
        int dir;
        float left = xPos + xRad - (other.xPos - other.xRad);
        float right = xPos - xRad - (other.xPos + other.xRad);
        float top = yPos + yRad - (other.yPos - other.yRad);
        float bottom = yPos - yRad - (other.yPos + other.yRad);
        if (!(left > 0 && (bottom > 0 ||  top > 0)) || (right > 0 && (bottom > 0 || top > 0))) return 0;

        float xMax = left > right ? left : right;
        float yMax = top > bottom ? top : bottom;

        return xMax > yMax ? 2 : 1;
    }
}
