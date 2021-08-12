//
// Created by lag on 28/12/18.
//

#ifndef PART_1_WAREHOUSE_CONNECTION_H
#define PART_1_WAREHOUSE_CONNECTION_H

#include "../utils_defines.h"

int OpenListenFD(char *str_port);
int AcceptConnection(SA_IN *client_addr_p, char *buffer, int listen_fd);


#endif //PART_1_WAREHOUSE_CONNECTION_H
