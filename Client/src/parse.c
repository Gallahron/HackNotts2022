#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "state.h"
#include "log.h"

#include "parse.h"

int parse_conn(struct State* state, char* msg, size_t msg_len);
bool parse_msg(struct State* state, const char* msg, size_t msg_len);
bool is_numeric(char c);

int parse_conn(struct State* state, char* msg, size_t msg_len)
{
	const char* accp_str = "ACCP(PN";
	size_t accp_str_len = strlen(accp_str);
	int player_number;

	if (strncmp(msg, accp_str, accp_str_len) != 0) {
		print_err("Did not receive correct response (ACCP(PN___))");
		return 0;
	}

	if ((player_number = strtol(msg + accp_str_len, &msg, 10)) <= 0) {
		print_err("Did not receive correct response, player number invalid (ACCP(PN___))");
		return 0;
	}

	if (strncmp(msg, ")", 1) != 0) {
		print_err("Did not receive correct response (ACCP(PN___))");
		return 0;
	}

	return player_number;
}

bool parse_msg(struct State* state, const char* msg, size_t msg_len)
{
	state->bullet_count = 0;
	state->player_count = 0;

	/*
	DATA(A(AT1_AN0_PX5767.653320_PY0.000000_SX32.000000_SY0.000000)A(AT0_AN1_PX0.000000_PY5574.660156_SX0.000000_SY32.000000))

	msg_type = get_msg_type(msg)

	if (msg_type == DATA) {
		

	    return true;
	} else {
		return false;
    }


	*/

	//return true;

	


	const char* c = msg;

	while (*c != ')') { 
		//Move to char after (
		while (*(++c) != '(');
		c++;

		struct Entity* entity = malloc(sizeof(struct Entity));
		int entity_type;
		int entity_number;
		int entity_state;
		
		while (*c != ')') {
			char* type = malloc(sizeof(char) * 2);
			char* type_ptr = type;
			char* value = malloc(sizeof(char) * 20);
			char* value_ptr = value;
			int index = 0;
			
			while (!is_numeric(*c) && index < 3) {
				type_ptr[index] = *c;
				c++;
				index++;
			}
			if (index == 3) {
				fprintf(stderr, "Error5");
				return false;
			}
			index = 0;
			while (*c != '_' && *c != ')' && index < 21) {
				value_ptr[index] = *c;
				c++;
				index++;
			}
			if (index == 21) {
				fprintf(stderr, "Error5");
				return false;
			}
			if (*c == '_') c++;

			switch (type[0]) {
				case 'A':
					switch(type[1]) {
						case 'T':
							entity_type = atoi(value);
							break;
						case 'N':
							entity_number = atoi(value);
							break;
						case 'X':
							entity_state = atoi(value);
							break;
						default:
							fprintf(stderr, "Error1");
							return false;
					}
					break;
				case 'P':
					switch(type[1]) {
						case 'X':
							entity->pos_x = atof(value);
							break;
						case 'Y':
							entity->pos_y = atof(value);
							break;
						default:
							fprintf(stderr, "Error2");
							return false;
					}
					break;
				case 'S':
					switch(type[1]) {
						case 'X':
							entity->speed_x = atof(value);
							break;
						case 'Y':
							entity->speed_y = atof(value);
							break;
						default:
							fprintf(stderr, "Error3");
							return false;
					}
					break;
				default:
					fprintf(stderr, "Error4");
					return false;
			}
			
		}

		switch (entity_type) {
			case 1: {
				struct Bullet* bullet = malloc(sizeof(struct Bullet));
				bullet->entity = *entity;
				switch (entity_number) {
					case 1:
						bullet->type = BL_BOMB;
						break;
					default:
						bullet->type = BL_STANDARD;
				}
				bullet->time_left = entity_state;

				state->bullets[state->bullet_count] = *bullet;
				state->bullet_count++;
				break;
			}
			case 0: {
				struct Player* player = malloc(sizeof(*player));
				player->entity = *entity;
				free(entity);
				player->player_number = entity_number;
				player->lives = entity_state;

				state->players[state->player_count] = *player;
				state->player_count++;
				break;
			}
		}
		c++;
	}

	for (int i = 0; i < (int) state->bullet_count; i++) {
		fprintf(stderr, "BULLET %u - X: %f, Y: %f\n", state->bullets[i].type, state->bullets[i].entity.pos_x, state->bullets[i].entity.pos_y);
	}
	for (int i = 0; i < (int) state->player_count; i++) {
		fprintf(stderr, "PLAYER %u - X: %f, Y: %f\n", state->players[i].player_number, state->players[i].entity.pos_x, state->players[i].entity.pos_y);
	}

	fprintf(stderr,"\n");
	//print_info("Trying to parse msg, but not done yet :(");
	//print_info("Msg: %.*s",msg_len, msg);

	return true;
}

bool is_numeric(char c)
{
	return c == '-' || (c >= '0' && c <= '9');
}
