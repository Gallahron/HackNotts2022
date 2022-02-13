#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "state.h"
#include "log.h"

#include "parse.h"

enum DataParsedType {
	DPT_ACTOR,
	DPT_FINISHED,
};

enum ActorType {
	AT_PLAYER = 0,
	AT_BULLET = 1,
};

struct Finished {
	int example_value;
};

int parse_conn(struct State* state, char* msg, size_t msg_len);
bool parse_msg(struct State* state, const char* msg, size_t msg_len);
const char* parse_actor(const char* msg, void** data_parsed, enum ActorType* data_parsed_type);
const char* parse_finished(const char* msg, void** data_parsed);

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


	const char msg_type_data_str[] = "DATA(";
	if (strncmp(msg, msg_type_data_str, sizeof(msg_type_data_str) - 1) != 0)
		return false;

	msg += sizeof(msg_type_data_str) - 1;

	while (*msg != ')' && *msg != '\0') {
		const char msg_subtype_actor_str[] = "A(";
		const char msg_subtype_finished_str[] = "F(";
		void* data_parsed;
		enum DataParsedType data_parsed_type;
		enum ActorType actor_type;
		if (strncmp(msg, msg_subtype_actor_str, sizeof(msg_subtype_actor_str) - 1) == 0) {
			data_parsed_type = DPT_ACTOR;
			msg = parse_actor(msg + sizeof(msg_subtype_actor_str) - 1, &data_parsed, &actor_type);
		} else if (strncmp(msg, msg_subtype_finished_str, sizeof(msg_subtype_finished_str) - 1) == 0) {
			data_parsed_type = DPT_FINISHED;
			msg = parse_finished(msg + sizeof(msg_subtype_finished_str) - 1, &data_parsed);
		} else {
			return false;
		}


		switch (data_parsed_type) {
		case DPT_ACTOR: {
			switch (actor_type) {
			case AT_PLAYER: {
				struct Player* player = (struct Player*) data_parsed;
				if (state->player_count < MAX_PLAYERS)
					state->players[state->player_count++] = *player;

				free(player);
				break;
			}
			case AT_BULLET: {
				struct Bullet* bullet = (struct Bullet*) data_parsed;
				if (state->bullet_count < MAX_BULLETS)
					state->bullets[state->bullet_count++] = *bullet;

				free(bullet);
				break;
			}
			}
			break;
		}
		case DPT_FINISHED: {
			struct Finished* finished = (struct Finished*) data_parsed;
			print_debug("Parsed example_value %d from F() message", finished->example_value);
			free(finished);
			break;
		}
		}
	}

	return true;
}

const char* parse_actor(const char* msg, void** data_parsed, enum ActorType* actor_type)
{
	char* msg_tmp;
	int p_at, p_an, p_ax;
	double p_px, p_py, p_sx, p_sy;

	while (true) {
		if (*msg == ')' || *msg == '\0') {
			break;
		} else if (strncmp(msg, "AT", 2) == 0) {
			msg += 2;
			p_at = strtol(msg, &msg_tmp, 10);
			msg = msg_tmp + 1;
		} else if (strncmp(msg, "AN", 2) == 0) {
			msg += 2;
			p_an = strtol(msg, &msg_tmp, 10);
			msg = msg_tmp + 1;
		} else if (strncmp(msg, "AX", 2) == 0) {
			msg += 2;
			p_ax = strtol(msg, &msg_tmp, 10);
			msg = msg_tmp + 1;
		}else if (strncmp(msg, "PX", 2) == 0) {
			msg += 2;
			p_px = strtod(msg, &msg_tmp);
			msg = msg_tmp + 1;
		} else if (strncmp(msg, "PY", 2) == 0) {
			msg += 2;
			p_py = strtod(msg, &msg_tmp);
			msg = msg_tmp + 1;
		} else if (strncmp(msg, "SX", 2) == 0) {
			msg += 2;
			p_sx = strtod(msg, &msg_tmp);
			msg = msg_tmp + 1;
		} else if (strncmp(msg, "SY", 2) == 0) {
			msg += 2;
			p_sy = strtod(msg, &msg_tmp);
			msg = msg_tmp + 1;
		} else {
			break;
		}
	}

	*actor_type = p_at;
	switch ((enum ActorType) p_at) {
	case AT_PLAYER: {
		struct Player* player;
		if (!(player = malloc(sizeof(*player)))) {
			print_err("Could not malloc player in parser");
			return NULL;
		}

		player->player_number = p_an;
		player->lives = p_ax;
		player->entity.pos_x = p_px;
		player->entity.pos_y = p_py;
		player->entity.speed_x = p_sx;
		player->entity.speed_y = p_sy;

		*data_parsed = (void*) player;

		break;
	}
	case AT_BULLET: {
		struct Bullet* bullet;
		if (!(bullet = malloc(sizeof(*bullet)))) {
			print_err("Could not malloc bullet in parser");
			return NULL;
		}

		bullet->type = p_an;
		bullet->time_left = p_ax;
		bullet->entity.pos_x = p_px;
		bullet->entity.pos_y = p_py;
		bullet->entity.speed_x = p_sx;
		bullet->entity.speed_y = p_sy;

		*data_parsed = (void*) bullet;

		break;
	}
	}

	if (*msg == ')')
		msg++;

	return msg;
}

const char* parse_finished(const char* msg, void** data_parsed)
{
	char* msg_tmp;
	int p_ex;

	while (true) {
		if (*msg == ')' || *msg == '\0') {
			break;
		} else if (strncmp(msg, "EX", 2) == 0) {
			msg += 2;
			p_ex = strtol(msg, &msg_tmp, 10);
			msg = msg_tmp + 1;
		} else {
			break;
		}
	}

	struct Finished* finished;
	if (!(finished = malloc(sizeof(*finished)))) {
		print_err("Could not malloc finished in parser");
		return NULL;
	}

	finished->example_value = p_ex;

	*data_parsed = (void*) finished;

	if (*msg == ')')
		msg++;

	return msg;
}
