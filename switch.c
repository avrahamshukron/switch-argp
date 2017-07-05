#include <syslog.h>

#include "switch.h"
#include "tcp.h"


static int connect_unix(char *path)
{
	syslog(LOG_DEBUG, "connecting to %s", path);
	return 0;
}

static int connect_device(char *path)
{
	syslog(LOG_DEBUG, "connecting to %s", path);
	return 0;
}

static int switch_connect(socket_type type, char *address)
{
	switch(type) {
	case UNIX:
		return connect_unix(address);
		break;
	case TCP:
		return tcp_connect(address, 12345);
		break;
	case DEV:
		return connect_device(address);
		break;
	default:
		return -1;
	}
}


int switch_main(struct infected_switch *sw)
{
	openlog(sw->args->name, LOG_PID | LOG_CONS, 0);
	if (sw->args->debug)
		setlogmask(LOG_UPTO(LOG_DEBUG));
	else
		setlogmask(LOG_UPTO(LOG_WARNING));
	syslog(LOG_INFO, "switch started");

	switch_connect(sw->args->sock_type, sw->args->source_addr);
}