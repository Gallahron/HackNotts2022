#include <stdlib.h>
#include <sysexits.h>
#include <stddef.h>
#include <stdbool.h>
#include <argp.h>
#include <argz.h>

#include "game.h"

const char *argp_program_version = "version 0.1.0";
const char *argp_program_bug_address = "<eden@etylermoss.com>";

struct Arguments {
	bool verbose;
	char* argz;
	size_t argz_len;
};

int main(int argc, char** argv);
int parse_opt(int key, char* arg, struct argp_state* state);

int main(int argc, char** argv)
{
	int exit_code = EX_OK;
	struct Arguments arguments = { 0 };

	const struct argp_option options[] = {
		{ 0, 0, 0, OPTION_DOC | OPTION_NO_USAGE, "OPTIONS:", 1 },
		{ "verbose", 'v', 0, OPTION_ARG_OPTIONAL, "Verbose output", 1 },
		{ 0, 0, 0, 0, 0, 0 },
	};

	const struct argp argp = { options, parse_opt, "[OPTION]...", NULL, NULL, NULL, NULL };

	if (argp_parse(&argp, argc, argv, 0, 0, &arguments) != 0) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	if (argz_count(arguments.argz, arguments.argz_len) != 0) {
		exit_code = EX_USAGE;
		goto cleanup;
	}

	if ((exit_code = game(arguments.verbose)) != EX_OK) {
		goto cleanup;
	}

cleanup:
	if (arguments.argz_len > 0)
		free(arguments.argz);

	return exit_code;
}

int parse_opt(int key, char* arg, struct argp_state* state)
{
	struct Arguments* arguments = (struct Arguments*) state->input;

	switch (key) {
	case 'v':
		arguments->verbose = true;
		break;
	case ARGP_KEY_ARG:
		argz_add(&arguments->argz, &arguments->argz_len, arg);
		break;
	case ARGP_KEY_INIT:
		arguments->argz = NULL;
		arguments->argz_len = 0;
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	
	return 0;
}
