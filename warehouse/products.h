//
// Created by lag on 30/12/18.
//

#ifndef WAREHOUSE_PRODUCTS_H
#define WAREHOUSE_PRODUCTS_H

#include "../warehouse.h"

int UpdateIncoming(products_list_t *list, product_t *products_incoming, type_amount_t *amounts,
                   int incoming_len_different, int len_products);
int UpdateOutlet(products_list_t *list, product_t *products_incoming, type_amount_t *amounts,
                 int incoming_len_different, int len_products);



int FitIn(products_list_t *list, product_t *products, type_amount_t *amounts, int len_amounts, int len_products);

int JoinToProductsStored(products_list_t *list, product_t *products, type_amount_t *amounts, int len_products);


#endif //WAREHOUSE_PRODUCTS_H
