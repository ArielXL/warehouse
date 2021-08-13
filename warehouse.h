//
// Created by lag on 26/12/18.
//

#ifndef PART_1_WAREHOUSE_WAREHOUSE_H
#define PART_1_WAREHOUSE_WAREHOUSE_H


#include "product_list/products_list.h"

//structs
typedef struct integer {
    char provider_id[3];     // identificador del productor
    int value;               // numero secuencial del productor
    char product_type[3];    // tipo de producto generado
    char product_data[4084];
}integer_t;

typedef struct type_amount{
    int count;
    char type[3];
}type_amount_t;

typedef struct params {
    products_list_t* products_info;
    int connection_fd;
} params_t;
//end structs

#endif //PART_1_WAREHOUSE_WAREHOUSE_H
