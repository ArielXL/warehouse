//
// Created by lag on 28/12/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "connection.h"
#include "../utils_defines.h"
#include "../debuging/debug.h"
#include "../entry_parser.h"
#include "../product_list/list_errors.h"


int OpenListenFD(char *str_port) {
    int port = parse_port(str_port, strlen(str_port));

    int listen_fd, opt_val = 1;
    SA_IN server_addr;

    /* Create a socket descriptor */
    Require((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0, "Socket"Exception, "OpenListenFD", "");
    /* Eliminates "Address already in use" error from bind */
    Require(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt_val, sizeof(int)) >= 0,
            "SocketOpt"Exception, "OpenListenFD", "");

    /* Listen fd will be an end point for all requests to port on any IP address for this host */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short) port);

    Require(bind(listen_fd, (SA *) &server_addr, sizeof(server_addr)) >= 0, "Bind"Exception, "OpenListenFD", "");

    /* Make it a listening socket ready to accept connection requests */
    Require(listen(listen_fd, 10) >= 0, "Listen"Exception, "OpenListenFD", "");

    return listen_fd;
}


int AcceptConnection(SA_IN *client_addr_p, char *buffer, int listen_fd) {
    int conn_fd;
    socklen_t sock_len = sizeof(*client_addr_p);

    /* accept: wait for a connection request */
    Require((conn_fd = accept(listen_fd, (SA *) client_addr_p, &sock_len)) > 0, "Accept"Exception, "AcceptConnection",
            "connection_fd its negative.");

    bzero(buffer, sizeof(char) * 8 + 1);

    Require(read(conn_fd, buffer, sizeof(char) * 8) > 0, "Read"Exception, "AcceptConnection", "initial name string too long.");

    WarehouseLog("%s%s%s%d%s%s", "Connection establish whit", buffer, "connection-fd:", conn_fd," address:", inet_ntoa(client_addr_p->sin_addr));
    return conn_fd;
}
