#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "tcp.h"

/**
 * Connect to a TCP port on a host.
 *
 * @ip 		NULL-terminated IPv4 addres string like "192.168.1.1"
 * @port	The remote port to connect to on the host
 *
 * Returns the socket's file descriptor, or a negative value in case of an
 * error.
 */
int tcp_connect(char *ip, uint16_t port)
{
	struct sockaddr_in server;
	int ret, sockfd;

	/* inet_pton returns 1 on success, 0 on failure */
	ret = inet_pton(AF_INET, ip, &server.sin_addr);
	if (!ret) {
		syslog(LOG_ERR, "Unable to parse IP from %s", ip);
		ret = -1;
		goto err_parse;
	}

	sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (sockfd < 0) {
		syslog(LOG_ERR, "Error createing socket: %m");
		ret = -errno;
		goto err_create_socket;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (ret = connect(sockfd, (struct sockaddr *)&server, sizeof(server))) {
		syslog(LOG_ERR, "Error connecting to server: %m");
		goto err_connect;
	}
	ret = sockfd;
	syslog(LOG_INFO, "Connected to server at %s:%d", ip, port);
	goto out;

err_connect:
	close(sockfd);
err_create_socket:
err_parse:
out:
	return ret;
}