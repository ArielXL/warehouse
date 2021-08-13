//
// Created by lag on 28/12/18.
//

#ifndef PART_1_WAREHOUSE_ENTRY_PARSER_H
#define PART_1_WAREHOUSE_ENTRY_PARSER_H

#include <string.h>
#include <stdlib.h>
#include "warehouse.h"

int find_char(char *argv, char c);

int parse_int(const char *string, long len, long start_index);

int parse_port(const char *string, long len);

void parse_amounts(int argc, char **argv, products_list_t *list);

void setTypeInfo(products_list_t *list, char *raw_data);

int contains_char(char *s, char c);


#endif //PART_1_WAREHOUSE_ENTRY_PARSER_H
