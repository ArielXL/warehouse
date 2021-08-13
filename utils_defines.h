//
// Created by lag on 28/12/18.
//

#ifndef PART_1_WAREHOUSE_UTILS_DEFINES_H
#define PART_1_WAREHOUSE_UTILS_DEFINES_H

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#define EQUALS(str1,str2) (!strcmp(str1, str2))
#define null_to_empty(arg) (arg == NULL ? "" : arg)
#define NoUpdated 0
#define Updated 1

#define COLOR_BLUE          "\x1b[36m"
#define COLOR_WHITE_BLUE    "\x1b[34m"
#define COLOR_YELLOW        "\x1b[33m"
#define COLOR_ROSE          "\x1b[35m"
#define COLOR_GREEN         "\x1b[32m"
#define COLOR_RED           "\x1b[31m"
#define COLOR_CLEAR         "\x1b[0m"
#define COLOR_NEG           "\x1b[1m"

#endif //PART_1_WAREHOUSE_UTILS_DEFINES_H
