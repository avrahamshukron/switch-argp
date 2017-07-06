#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <argp.h>

#include "switch.h"

#define OPT_SERVER (333) /* Just some non-printable int */
#define DEFAULT_SERVER_PATH "/var/run/switch"

const char *argp_program_version = "switch 1.0";

/* A description of the positional arguments we accept. */
static const char args_doc[] = "NAME ID SOCK_TYPE ADDRESS";

/* Program documentation. */
static const char doc[] =
	"Starts an Infected Switch\v"
	"  NAME         The name of this switch\n"
	"  ID           The Infected DST which the switch binds to, as a 2-byte\n"
	"               hex string, like 0xfa9b\n"
	"  SOCK_TYPE    How the switch will connect to it's source. Valid\n"
	"               options are 'tcp', 'unix' or 'dev'\n"
	"  ADDRESS      The address for the infected source. The value is\n"
	"               dependant on the SOCK_TYPE specified. For 'unix', 'dev'\n"
	"               this should be a UNIX path. For 'tcp' this should be\n"
	"               an <IP>:<PORT> string\n";

/* Keys for options without short-options. */
typedef enum {
	ARG_NAME,
	ARG_ID,
	ARG_SOCK_TYPE,
	ARG_ADDRESS,
	ARG_LAST /* Terminator. Add entries before only */
} argument;

/* The options we understand. */
static struct argp_option options[] = {
	{"stream", 's', 0, 0,
	 "Whether the input source is a stream (not seqpacket)"},
	{"router", 'r', "PATH", 0,
	 "The unix socket address of the Infected Router"},
	{"server", OPT_SERVER, "PATH", 0,
	 "The unix socket address created for clients by this switch"},
	{"debug", 'd', 0, 0,
	 "Enable debug log output"},
	{ 0 }
};

/**
 * forward declaration
 */
static int parse_opt(int key, char *arg, struct argp_state *state);

/* Our argp parser. */
static struct argp argp = {
	options,
	parse_opt,
	args_doc,
	doc
};

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

static struct switch_args args = {
	.stream = 0,
	.debug = 0,
	.router_addr = NULL,
	.server_path = DEFAULT_SERVER_PATH
};

int main(int argc, char **argv)
{
	struct infected_switch sw;
	argp_parse(&argp, argc, argv, 0, 0, &args);
	printf("name = %s\n", args.name);
	printf("id = 0x%04x\n", args.id);
	printf("socket_type = %d\n", args.sock_type);
	printf("address = %s\n", args.source_addr);
	printf("server = %s\n", args.server_path);
	printf("stream = %d\n", args.stream);
	printf("router = %s\n", args.router_addr);
	printf("debug = %d\n", args.debug);

	switch_init(&sw, args);
	switch_start(&sw);
	exit(0);
}