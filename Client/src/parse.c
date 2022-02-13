#include <stdbool.h>
#include <stddef.h>

#include "state.h"
#include "log.h"

#include "parse.h"

bool parse_msg(struct State* state, const char* msg, size_t msg_len);

bool parse_msg(struct State* state, const char* msg, size_t msg_len)
{
	print_info("Trying to parse msg, but not done yet :(");
	print_info("Msg: %.*s", msg_len, msg);

	return true;
}
