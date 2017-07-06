#ifndef SWITCH_H
#define SWITCH_H

#include <stdint.h>

typedef enum {
	UNIX,
	TCP,
	DEV,
	UNKNOWN = -1
} socket_type;

/* Used by main to communicate with parse_opt. */
struct switch_args {
	char *name;
	uint16_t id;
	socket_type sock_type;
	char *source_addr;
	char *server_path;
	char *router_addr;
	uint8_t stream;
	uint8_t debug;
};

/*
 * Internal switch state representation
 */
struct infected_switch {
	int source_fd; /* FD for the Infected source */
	struct switch_args args;
};

/**
 * Initialize an Infected Switch
 * @param sw:	Empty `struct infected_switch` to be used later in any call to
* 		the switch.
 * @param args:	Pre-populated `struct switch_args` with the desired arguments.
 */
void switch_init(struct infected_switch *sw, struct switch_args args);

int switch_start(struct infected_switch *sw);

#endif /* SWITCH_H */