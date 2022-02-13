#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdbool.h>
#include <stddef.h>

#include "state.h"

int parse_conn(struct State* state, char* msg, size_t msg_len);
bool parse_msg(struct State* state, const char* msg, size_t msg_len);

#endif
