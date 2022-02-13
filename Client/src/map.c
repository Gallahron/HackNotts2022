#include <stdio.h>
#include <stdlib.h>

#include "log.h"

#include "map.h"

struct MapData* map_init(enum Map map);
void map_destroy(struct MapData* map_data);

struct MapData* map_init(enum Map map)
{
	if (map == MP_STANDARD) {
		struct MapData* map_data;
		if (!(map_data = calloc(1, sizeof(*map_data))))
			return NULL;

		map_data->map = map;

		map_data->blocks[BLOCK_INDEX(0, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(1, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(2, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(3, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(4, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(5, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(6, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(7, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(8, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(9, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(10, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(11, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(12, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(13, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(14, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(15, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(16, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(17, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(18, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(19, 0)] = BK_GROUND;
		map_data->blocks[BLOCK_INDEX(4, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(5, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(6, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(7, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(8, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(9, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(10, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(11, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(12, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(13, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(14, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(15, 4)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(2, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(3, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(4, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(5, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(14, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(15, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(16, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(17, 8)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(8, 11)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(9, 11)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(10, 11)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(11, 11)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(0, 12)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(1, 12)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(2, 12)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(3, 12)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(16, 12)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(17, 12)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(18, 12)] = BK_PLATFORM;
		map_data->blocks[BLOCK_INDEX(19, 12)] = BK_PLATFORM;

		return map_data;
	} else {
		return NULL;
	}
}

void map_destroy(struct MapData* map_data)
{
	if (map_data)
		free(map_data);
}
