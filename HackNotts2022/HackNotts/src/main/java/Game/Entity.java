package Game;

public class Entity {
    public boolean useGravity;
    public boolean grounded = false;

    private int entityType;
    private int entityNumber;

    private float xPos;
    private float yPos;

    private float xSpeed;
    private float ySpeed;

    private float xSize;
    private float ySize;


    public Entity(float x, float y, float xSpeed, float ySpeed, float xSize, float ySize, int entityNumber, int entityType, boolean useGravity) {
        this.xPos = x;
        this.yPos = y;
        this.xSpeed = xSpeed;
        this.ySpeed = ySpeed;
        this.xSize = xSize;
        this.ySize = ySize;
        this.entityNumber = entityNumber;
        this.entityType = entityType;
        this.useGravity = useGravity;
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
    public void modXSpeed(float speed) {
        this.xSpeed += speed;
    }

    public float getYSpeed() {
        return ySpeed;
    }

    public void setYSpeed(float ySpeed) {
        this.ySpeed = ySpeed;
    }
    public void modYSpeed(float speed) {
        this.ySpeed += speed;
    }

    public float getXRad() {
        return xSize;
    }

    public void setXRad(float xRad) {
        this.xSize = xRad;
    }

    public float getYRad() {
        return ySize;
    }

    public void setYRad(float yRad) {
        this.ySize = yRad;
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
        //System.out.println("X Y "+ xPos + " " + yPos);
        if (other.pointInEntity(xPos , yPos + ySize/8) || other.pointInEntity(xPos, yPos + 7 * ySize/8)) return 0;
        if (other.pointInEntity(xPos + xSize , yPos + ySize/8) || other.pointInEntity(xPos + xSize, yPos + 7 * ySize/8) ) return 2;
        if (other.pointInEntity(xPos + 7*xSize/8, yPos) || other.pointInEntity(xPos + xSize/8, yPos)) return 1;
        if (other.pointInEntity(xPos + 7*xSize/8, yPos + ySize) || other.pointInEntity(xPos + xSize/8, yPos+ySize)) return 3;

        if (swap) {
            return other.isCollided(this, false);
        }
        return -1;
    }

    public int isCollided(Entity other) {
        return isCollided(other, true);
    }

    public boolean pointInEntity(float x, float y) {
        /*System.out.println("1: " + x + " " + (xPos + xRad));
        System.out.println("2: " + x + " " + (xPos - xRad));
        System.out.println("3: " + y + " " + (yPos + yRad));
        System.out.println("4: " + y + " " + (yPos - yRad));*/

        if (x <= xPos + xSize && x >= xPos && y <= yPos + ySize && y >= yPos) {
            return true;
        }
        return false;
    }
}
