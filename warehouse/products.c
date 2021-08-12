//
// Created by lag on 30/12/18.
//

#include <strings.h>
#include <stdlib.h>
#include "products.h"
#include "../product_list/list_errors.h"
#include "../debuging/debug.h"

int JoinToProductsStored(products_list_t *list, product_t *products, type_amount_t *amounts, int len_products) {
    NotNullListRequired(list, "JoinToProductsStored");
    NotNullRequired(products, "JoinToProductsStored", "products its not declared or its null");
    NotNullRequired(amounts, "JoinToProductsStored", "amounts its not declared or its null");
    Require(len_products > 0, Exception, "JoinToProductsStored", "len_products must be greater than 0");


    // WarehouseLog("%s%d%s%d","--------count_differents",list->state.count_different, "--------len_products", len_products);

    // for (int i = 0; i < list->state.count_different; ++i) {
    //     WarehouseLog("%s%s%d%s%d",list->state.amounts[i].product_types,"-",list->state.amounts[i].capacity,"/",list->state.amounts[i].amount);
    // }
    int success = 0;
    for (int i = 0; i < len_products; ++i) {
        // WarehouseLog("%s%s%s", "Product ->", products[i].product_type,"<-");
        success |= AddToList(list, &products[i]);
    }
    // WarehouseLog("%s%d", "Success ->", success);

    Require(success == 0, "ProductFormat"Exception, "JoinToProductsStored", "The products received are corrupted");

    return success;
}

int ExtractFromProductsStored(products_list_t *list, product_t *products, type_amount_t *amounts, int len_types) {
    NotNullListRequired(list, "ExtractFromProductsStored");
    NotNullRequired(products, "ExtractFromProductsStored", "products its not declared or its null");
    NotNullRequired(amounts, "ExtractFromProductsStored", "amounts its not declared or its null");
    Require(len_types > 0, Exception, "ExtractFromProductsStored", "len_types must be greater than 0");

    // for (int i = 0; i < list->state.count_different; ++i) {
    //     WarehouseLog("%s%s%d%s%d",list->state.amounts[i].product_types,"-",list->state.amounts[i].capacity,"/",list->state.amounts[i].amount);
    // }

    int success = 0;
    for (int i = 0; i < len_types; ++i){
        // WarehouseLog("%s%s%s", "Product ->", amounts[i].type, "<-");
        success |= DeleteOfType(list, amounts[i].count, amounts[i].type);
    }
    // WarehouseLog("%s%d", "Success ->", success);

    Require(success == 0, "ProductFormat"Exception, "ExtractFromProductsStored", "The products stored are corrupted");

    return success;
}

//0-->cabe (>0)-->no cabe
int FitIn(products_list_t *list, product_t *products, type_amount_t *amounts, int len_amounts, int len_products) {
    NotNullListRequired(list, "FitIn");
    NotNullRequired(products, "FitIn", "products its not declared or its null");
    NotNullRequired(amounts, "FitIn", "amounts its not declared or its null");
    Require(len_amounts > 0, Exception, "FitIn", "len_amounts must be greater than 0");

    int total = 0, total_surplus = 0, index_state = -1;

    /* this type fit with the capacity in general? */
    total = list->state.total_amount + len_products;
    // WarehouseLog("%d%d%s%d",list->state.total_amount, amounts[i].count,"list->state.total_amount + amounts[i].count -- this type fit with the capacity in general?", total);
    if (total > list->state.total_capacity) {
        total_surplus++;
    } else{
        for (int i = 0; i < len_amounts; ++i) {

            /* this type exist in list? */
            index_state = indexOf_state(&list->state, amounts[i].type);
            if (index_state < 0) {
                if(list->state.restricted) {
                    total_surplus++;
                    continue;
                }
                WarehouseLog("%s%s", "created new type -", amounts[i].type);
                index_state = CreateTypeSpace(&list->state, amounts[i].type, 0);
            }

            /* this type fit with the specific type amount? */
            total = list->state.amounts[index_state].amount + amounts[i].count;
            // WarehouseLog("%d%d%s%d",list->state.amounts[index_state].amount , amounts[i].count,"list->state.amounts[index_state].amount + amounts[i].count -- this type fit with the specific type amount?", total);
            if (list->state.amounts[index_state].capacity && total > list->state.amounts[index_state].capacity) {
                total_surplus++;
                continue;
            }
        }
    }
    // WarehouseLog("%s","------------FitIn INCOMING----------------");

    // for (int i = 0; i < len_amounts; ++i) {
    //     WarehouseLog("%s%d",amounts[i].type,amounts[i].count);
    // }
    // WarehouseLog("%s%d","------------FitIn STORED----------------", list->state.count_different);

    // for (int i = 0; i < list->state.count_different; ++i) {
    //     WarehouseLog("%s%s%d%s%d",list->state.amounts[i].product_types,"-",list->state.amounts[i].amount,"/",list->state.amounts[i].capacity);
    // }
    // WarehouseLog("%s%d","------------FitIn-return----------------", total_surplus);

    return total_surplus;
}

int AreEnough(products_list_t *list, product_t *products, type_amount_t *amounts, int len_amounts, int len_products) {
    NotNullListRequired(list, "AreEnough");
    NotNullRequired(products, "AreEnough", "products its not declared or its null");
    NotNullRequired(amounts, "AreEnough", "amounts its not declared or its null");

    Require(len_amounts > 0, Exception, "AreEnough", "len_amounts must be greater than 0");
    Require(len_products > 0, Exception, "AreEnough", "len_products must be greater than 0");

    int exceed = 0, total_left = 0;

    if(len_products > list->count)
        return len_products - list->count;

    for (int i = 0; i < len_amounts; ++i) {
        exceed = AmountOfType(list, amounts[i].type) - amounts[i].count;
        if (exceed < 0)
            total_left += exceed;
    }
    return (total_left < 0) ? -total_left : 0;
}

int UpdateIncoming(products_list_t *list, product_t *products_incoming, type_amount_t *amounts,
                   int incoming_len_different, int len_products) {
    NotNullListRequired(list, "UpdateIncoming");
    NotNullRequired(products_incoming, "UpdateIncoming", "products_incoming its not declared or its null");
    NotNullRequired(amounts, "UpdateIncoming", "amounts its not declared or its null");
    Require(incoming_len_different > 0 && len_products, Exception, "UpdateIncoming", "len's must be greater than 0");


    int extra_amount;
    /* Fit? */
    if ((extra_amount = FitIn(list, products_incoming, amounts, incoming_len_different,len_products)) != 0) {
        // WarehouseLog("%s%d","extra amount needed -->", extra_amount);
        return extra_amount;
    }
    JoinToProductsStored(list, products_incoming, amounts, len_products);
    return 0;
}

int UpdateOutlet(products_list_t *list, product_t *products_incoming, type_amount_t *amounts,
                 int incoming_len_different, int len_products) {
    NotNullListRequired(list, "UpdateOutlet");
    NotNullRequired(products_incoming, "UpdateOutlet", "products_incoming its not declared or its null");
    NotNullRequired(amounts, "UpdateOutlet", "amounts its not declared or its null");
    Require(incoming_len_different > 0 && len_products, Exception, "UpdateOutlet", "len's must be greater than 0");

    int missing_amount;
    /* There are? */
    if ((missing_amount = AreEnough(list, products_incoming, amounts, incoming_len_different, len_products)) != 0) {
        // WarehouseLog("%s%d", "missing amount needed -->", missing_amount);        
        return missing_amount;
    }
    ExtractFromProductsStored(list, products_incoming, amounts, incoming_len_different);
    return 0;
}