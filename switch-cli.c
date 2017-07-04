#include <stdlib.h>
#include <error.h>
#include <string.h>

#include "switch-cli.h"

static socket_type parse_socket_type(char *name)
{
	if (strcmp(name, "unix") == 0) {
		return UNIX;
	} else if (strcmp(name, "tcp") == 0) {
		return TCP;
	} else if (strcmp(name, "dev") == 0) {
		return DEV;
	}

	return UNKNOWN;
}

static int parse_arg(char *arg, struct argp_state *state)
{
	struct switch_args *args = state->input;

	switch (state->arg_num) {
	case ARG_NAME:
		args->name = arg;
		break;
	case ARG_ID:
		errno = 0;
		args->id = (uint16_t)strtoul(arg, NULL, 0);
		if (errno)
			argp_error(state, "Invalid ID");
		break;
	case ARG_SOCK_TYPE:
		args->sock_type = parse_socket_type(arg);
		if (args->sock_type == UNKNOWN)
			argp_error(state, "Invalid socket type");
		break;
	case ARG_ADDRESS:
		args->source_addr = arg;
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* Parse a single option. */
static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct switch_args *args = state->input;

	switch (key) {
	case ARGP_KEY_ARG:
		if (parse_arg(arg, state))
			return ARGP_ERR_UNKNOWN;
	case 's':
		args->stream = 1;
		break;
	case 'r':
		args->router_addr = arg;
		break;
	case 'd':
		args->debug = 1;
		break;
	case ARGP_KEY_NO_ARGS:
	case ARGP_KEY_END:
		if (state->arg_num < ARG_LAST)
			argp_usage(state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

int main(int argc, char **argv)
{
	struct switch_args args = {
		.stream = 0,
		.debug = 0,
		.router_addr = NULL,
		.server_path = DEFAULT_SERVER_PATH
	};

	argp_parse(&argp, argc, argv, 0, 0, &args);
	printf("name = %s\n", args.name);
	printf("id = 0x%04x\n", args.id);
	printf("socket_type = %d\n", args.sock_type);
	printf("address = %s\n", args.source_addr);
	printf("server = %s\n", args.server_path);
	printf("stream = %d\n", args.stream);
	printf("router = %s\n", args.router_addr);
	printf("debug = %d\n", args.debug);

	switch_main(args);
	exit(0);
}