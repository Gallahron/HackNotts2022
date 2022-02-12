#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>
#include <arpa/inet.h>

int game(struct in_addr server_addr, bool verbose);

#endif
