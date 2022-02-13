package Game;

import java.util.ArrayList;

public class Arena {
    public ArrayList<Block> blocks = new ArrayList<Block>();
    public final int maxArenaX = 20;
    public final int maxArenaY = 15;

    public String mapData[];

    public Arena() {
        mapData = new String[] {
                "                    ",
                "                    ",
                "XXXX            XXXX",
                "        XXXX        ",
                "                    ",
                "                    ",
                "  XXXX        XXXX  ",
                "                    ",
                "                    ",
                "                    ",
                "    XXXXXXXXXXXX    ",
                "                    ",
                "                    ",
                "                    ",
                "XXXXXXXXXXXXXXXXXXXX"
        };
        ParseMap(mapData);
    }

    public void AddBlock(float x, float y) {
        blocks.add(new Block(x, y));
    }

    public void ParseMap(String[] data) {
        for (int x = 0; x < maxArenaX; x++) {
            for (int y = 0; y < maxArenaY; y++) {
                if (data[maxArenaY - y - 1].charAt(x) == 'X') AddBlock(x, y);
            }
        }
    }
}
