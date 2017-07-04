#include <syslog.h>
#include <sys/socket.h>

#include "switch.h"

int switch_main(struct switch *switch)
{
	openlog(switch->args->name, LOG_PID | LOG_CONS, 0);
	if (switch->args->debug)
		setlogmask(LOG_UPTO(LOG_DEBUG));
	else
		setlogmask(LOG_UPTO(LOG_WARNING));
	syslog(LOG_INFO, "switch started");

	connect(args.sock_type, args.source_addr);
}

static int connect_tcp(char *address)
{
	struct sockaddr addr;
	int sockfd, ret;

	if (ret = parse_inet_address(address, &addr)) {
		syslog(LOG_ERR, "Unable to parse IP:port from %s", address);
		goto err_parse;
	}

	sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (sockfd < 0) {
		syslog(LOG_ERR, "Error createing socket: %m", sockfd);
		goto err_create_socket;
	}

	if (ret = connect(sockfd, &addr, sizeof(addr))) {
		syslog(LOG_ERR, "Error connecting to server: %m",ret);
		goto err_connect;
	}
	return 0;

err_connect:
	close(sockfd);
err_create_socket:
err_parse:
	return ret;
}

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

static int connect(socket_type type, char *address)
{
	switch(type) {
	case UNIX:
		return connect_unix(address);
		break;
	case TCP:
		return connect_tcp(address);
		break;
	case DEV:
		return connect_device(address);
		break;
	default:
		return -EINVAL;
	}
}