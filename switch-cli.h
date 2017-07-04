#ifndef SWITCH_CLI_H
#define SWITCH_CLI_H

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

#endif /* SWITCH_CLI_H */