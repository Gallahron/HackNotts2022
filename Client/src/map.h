#ifndef __MAP_H__
#define __MAP_H__

#define MAP_SIZE_X 20
#define MAP_SIZE_Y 15
#define MAP_SIZE MAP_SIZE_X * MAP_SIZE_Y

#define BLOCK_INDEX(x, y) (x * MAP_SIZE_Y + (MAP_SIZE_Y - y)) /* convert bottom left 0,0 to top right 0,0 1D array coordinates */

enum Map {
	MP_STANDARD = 0,
};

enum Block {
	BK_AIR = 0,
	BK_GROUND,
	BK_PLATFORM,
};

struct MapData {
	enum Map map;
	enum Block blocks[MAP_SIZE + 1];
};

struct MapData* map_init(enum Map map);
void map_destroy(struct MapData* map_data);

#endif
