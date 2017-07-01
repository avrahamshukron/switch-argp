#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <error.h>
#include <argp.h>
#include <string.h>

#define OPT_SERVER (333) /* Just some non-printable int */
#define DEFAULT_SERVER_PATH "/var/run/switch"

const char *argp_program_version = "switch 1.0";

/* A description of the positional arguments we accept. */
static const char args_doc[] = "NAME ID SOCK_TYPE ADDRESS";

/* Program documentation. */
static const char doc[] =
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


typedef enum {
	UNIX,
	TCP,
	DEV,
	UNKNOWN = -1
} socket_type;

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

/* Used by main to communicate with parse_opt. */
struct arguments {
	char *name;
	uint16_t id;
	socket_type sock_type;
	char *source_addr;
	char *server_path;
	char *router_addr;
	uint8_t stream;
	uint8_t debug;
};

static int parse_arg(char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (state->arg_num) {
	case ARG_NAME:
		arguments->name = arg;
		break;
	case ARG_ID:
		errno = 0;
		arguments->id = (uint16_t)strtoul(arg, NULL, 0);
		if (errno) {
			argp_error(state, "Invalid ID");
		}
		break;
	case ARG_SOCK_TYPE:
		arguments->sock_type = parse_socket_type(arg);
		break;
	case ARG_ADDRESS:
		arguments->source_addr = arg;
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* Parse a single option. */
static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key) {
	case ARGP_KEY_ARG:
		if (parse_arg(arg, state))
			return ARGP_ERR_UNKNOWN;
	case 's':
		arguments->stream = 1;
		break;
	case 'r':
		arguments->router_addr = arg;
		break;
	case 'd':
		arguments->debug = 1;
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

/* Our argp parser. */
static struct argp argp = {
	options,
	parse_opt,
	args_doc,
	doc
};

int main(int argc, char **argv)
{
	struct arguments arguments = {
		.stream = 0,
		.debug = 0,
		.router_addr = NULL,
		.server_path = DEFAULT_SERVER_PATH
	};

	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	printf("name = %s\n", arguments.name);
	printf("id = 0x%04x\n", arguments.id);
	printf("socket_type = %d\n", arguments.sock_type);
	printf("address = %s\n", arguments.source_addr);
	printf("server = %s\n", arguments.server_path);
	printf("stream = %d\n", arguments.stream);
	printf("router = %s\n", arguments.router_addr);
	printf("debug = %d\n", arguments.debug);
	exit(0);
}
