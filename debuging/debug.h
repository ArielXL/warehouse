//
// Created by lag on 28/12/18.
//

#ifndef PART_1_WAREHOUSE_DEBUG_H
#define PART_1_WAREHOUSE_DEBUG_H

#include "../product_list/products_list.h"

#define PRINT_INT(n) printf("%d ", n)
#define PRINT_STR(str) printf("%s ", str)
#define PRINT_CHR(c) printf("%c ", c)

void WarehouseLog(const char *string_format, ...);

void PrintState(products_list_t *list, int num_id, int explicit);

void ConsumerLog(const char *string_format, ...);

void ProducerLog(const char *string_format, ...);

#endif //PART_1_WAREHOUSE_DEBUG_H
