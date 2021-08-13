#include "product_list/products_list.h"
#include "warehouse.h"
#include "utils_defines.h"
#include "warehouse/main.h"
#include "entry_parser.h"
#include "product_list/list_errors.h"
#include "debuging/debug.h"

#ifndef PRODUCER_CONSUMER_H
      
    #define PRODUCER_CONSUMER_H 1

    int get_port(int argc, char **argv);
    char *get_hostname(int argc, char **argv);
    void generate_default(type_amount_t *type_amount);
    void get_product_type(char *argv, char *let_type);
    int get_number(char *argv);
    void PrintStates(product_t *product, type_amount_t *type_amount, int count_types);
    void ParseTypesAmount(type_amount_t *type_amount, int count_product, int *len_products,
                          char **argv, int is_random);
    int OpenClientFD(char *hostname, int port);
    int EstablishConnection(char *hostname, int port);
    void FillProducts(product_t *products, type_amount_t *amounts, int len_products, int len_types);
    
#endif