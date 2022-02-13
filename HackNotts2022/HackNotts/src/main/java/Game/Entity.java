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

    public int isCollided(Entity other, boolean swap) {
        if (other.pointInEntity(xPos + xRad, yPos) || other.pointInEntity(xPos - xRad, yPos) ) return 2;
        if (other.pointInEntity(xPos, yPos + yRad) || other.pointInEntity(xPos, yPos - yRad)) return 1;

        if (swap) {
            return isCollided(this, false);
        }
        return 0;
    }

    public int isCollided(Entity other) {
        return isCollided(other, true);
    }

    public boolean pointInEntity(float x, float y) {
        if (x < xPos + xRad && x > xPos - yRad && y < yPos + yRad && y > yPos - yRad) {
            return true;
        }
        return false;
    }
}
