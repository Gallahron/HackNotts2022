#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <stddef.h>
#include <stdbool.h>
#include <argp.h>
#include <argz.h>
#include <arpa/inet.h>
#include <time.h>

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
	struct in_addr server_addr;

	const struct argp_option options[] = {
		{ "IP", 0, 0, OPTION_DOC | OPTION_NO_USAGE, "IPv4 Address of server (defaults to 127.0.0.1)", 0 },
		{ 0, 0, 0, OPTION_DOC | OPTION_NO_USAGE, "OPTIONS:", 1 },
		{ "verbose", 'v', 0, OPTION_ARG_OPTIONAL, "Verbose output", 1 },
		{ 0, 0, 0, 0, 0, 0 },
	};

	const struct argp argp = { options, parse_opt, "[IP]", NULL, NULL, NULL, NULL };

	if (argp_parse(&argp, argc, argv, 0, 0, &arguments) != 0) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	size_t argument_count = argz_count(arguments.argz, arguments.argz_len);
	if (argument_count > 1) {
		exit_code = EX_USAGE;
		goto cleanup;
	}

	if (argument_count == 0) {
		inet_aton("127.0.0.1", &server_addr);
	} else {
		if (inet_aton(arguments.argz, &server_addr) == 0) {
			exit_code = EX_USAGE;
			goto cleanup;
		}

		/* must be 3 '.' chars in ip string */

		int dot_count = 0;
		for (char* c = arguments.argz; *c != '\0'; c++)
			if (*c == '.')
				dot_count++;

		if (dot_count != 3) {
			exit_code = EX_USAGE;
			goto cleanup;
		}
	}

	srand(time(NULL));

	if ((exit_code = game(server_addr, arguments.verbose)) != EX_OK) {
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
