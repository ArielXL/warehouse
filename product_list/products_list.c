//
// Created by lag on 30/12/18.
//

#include <strings.h>
#include <string.h>
#include <stdio.h>
#include "products_list.h"
#include "list_errors.h"

void resizeState(products_state_t *state, int new_len);

int cmp_types(const char *type1, const char *type2);

//new

void InitState(products_state_t *state, int initial_len) {
    NotNullRequired(state, "InitState", "state its not declared or its null");

    if (initial_len < 1)
        initial_len = 1;

    bzero(state, sizeof(products_state_t));

    state->arrayCount = initial_len;

    state->amounts = malloc(sizeof(tuple_amount_type_t) * initial_len + 4);
    bzero(state->amounts, sizeof(tuple_amount_type_t) * initial_len + 2);

}


//i->indice del tipo
//-1->no se encontro
int indexOf_state(products_state_t *state, char *product_type) {
    NotNullRequired(state, "IndexOf_state", "state its not declared or its null");
    NotNullRequired(state->amounts, "IndexOf_state", "state->amounts its not declared or its null");
    NotNullRequired(product_type, "IndexOf_state", "product_type its not declared or its null");

    for (int i = 0; i < state->count_different; ++i) {
        if (!cmp_types(state->amounts[i].product_types, product_type)) {
            return i;
        }
    }
    printf("---type not found---\n");
    return -1;
}


int updateStateAdd(products_state_t *state, char *product_type) {
    NotNullRequired(state, "updateStateAdd", "state its not declared or its null");
    NotNullRequired(product_type, "updateStateAdd", "product_type its not declared or its null");
    //comprobaciones

    int type_ind = indexOf_state(state, product_type);
    if (type_ind is -1 and not state->restricted)
        type_ind = CreateTypeSpace(state, product_type, 0);

    if (ThereArePlace(state, type_ind)) {
        state->amounts[type_ind].amount++;
        state->total_amount++;
        printf("---Se pubo annadir %s---\n", product_type);
        return 0;
    }
    printf("---No se pubo annadir %s---\n", product_type);
    return 1;
}

int updateStateDelete(products_state_t *state, char *product_type) {
    NotNullRequired(state, "updateStateDelete", "state its not declared or its null");
    NotNullRequired(product_type, "updateStateDelete", "product_type its not declared or its null");
    //comprobaciones

    int type_ind = indexOf_state(state, product_type);
    if (ThereAreSome(state, type_ind)) {
        state->total_amount--;
        state->amounts[type_ind].amount--;
        printf("---Se pubo extraer %s---\n", product_type);
        return 0;
    }
    printf("---No se pubo extraer %s---\n", product_type);

    return 1;
}

//i->indice del nuevo tipo
//-1->no se puede
int CreateTypeSpace(products_state_t *state, char *type, int capacity) {
    NotNullRequired(state, "CreateTypeSpace", "state its not declared or its null");
    NotNullRequired(state->amounts, "CreateTypeSpace", "state->amounts its not declared or its null");
    NotNullRequired(type, "CreateTypeSpace", "type its not declared or its null");
    Require(capacity >= 0, StateListException, "CreateTypeSpace", "Capacity its lesser than zero.");
    Require(capacity != 0 || !state->restricted, StateListException, "CreateTypeSpace", "new types not allowed");
    //comprobaciones


    if (state->count_different == state->arrayCount)
        resizeState(state, state->arrayCount == 0 ? 1 : state->arrayCount * 2);
    state->count_different++;

    bzero(&state->amounts[state->count_different - 1], sizeof(tuple_amount_type_t));
    strncpy(state->amounts[state->count_different - 1].product_types, type, 3);

    state->amounts[state->count_different - 1].capacity = capacity;

    printf("-> %d/%d\n",state->amounts[state->count_different - 1].amount,state->amounts[state->count_different - 1].capacity);
    return state->count_different - 1;
}

void resizeState(products_state_t *state, int new_len) {
    NotNullRequired(state, "resizeState", "state its not declared or its null");
    NotNullRequired(state->amounts, "resizeState", "state->amounts its not declared or its null");
    Require(new_len > state->arrayCount, StateListException, "resizeState", "new len must be greater than the old");

    state->amounts = realloc(state->amounts, sizeof(tuple_amount_type_t) * new_len + 4);
    state->arrayCount = new_len;
}

void SetTotalCapacity(products_list_t *list, int total_capacity) {
    NotNullListRequired(list, "SetTotalCapacity");
    NotNullRequired(list->state.amounts, "SetTotalCapacity", "state->amounts its not declared or its null");
    Require(total_capacity >= 0, StateListException, "SetTotalCapacity", "total_capacity its lesser than zero.");


    list->state.restricted = !total_capacity;

    if (total_capacity != 0) {
        list->state.total_capacity = total_capacity;
        return;
    }
    int total = 0;
    for (int i = 0; i < list->state.count_different; ++i)
        total += list->state.amounts[i].capacity;

    list->state.total_capacity = total;
}

//ens new

void InitList(products_list_t *list, int initial_len, size_t size_type) {
    NotNullRequired(list, "InitList", "products_list its not declared or its null");

    if (initial_len < 1)
        initial_len = 1;

    bzero(list, sizeof(products_list_t));

    list->count = 0;
    list->arrayCount = initial_len;
    list->sizeType = size_type;
    list->content = malloc(size_type * initial_len + 4);

    //new
    bzero(&list->state, sizeof(products_state_t));
    InitState(&list->state, initial_len);
    //end new
}

void ClearList(products_list_t *list) {
    NotNullListRequired(list, "ClearList");

    //new
    if (list->state.arrayCount != 0) {
        free(list->state.amounts);
    }
    bzero(&list->state, sizeof(products_state_t));
    //end new

    free(list->content);
    bzero(list, sizeof(products_list_t));
}

void resizeList(products_list_t *list, int new_len) {
    NotNullListRequired(list, "resizeList");
    Require(new_len > list->arrayCount, ListException, "resizeList", "products_list->arrayCount >= new_len");

    list->arrayCount = new_len;
    list->content = realloc(list->content, list->sizeType * list->arrayCount + 4);
}

//1 ->cabe 0 ->no cabe
int setProductAt(products_list_t *list, int index, void *value) {
    //new
    if (!updateStateAdd(&list->state, (*(product_t *) value).product_type))
        return 0;
    //end new
    list->content[index] = *(product_t *) value;
    return 1;
}

int SetAt(products_list_t *list, int ind, void *value) {
    NotNullListRequired(list, "SetAt");
    IndexRangeRequired(list, ind, "SetAt");
    //todo change it for another types
    return setProductAt(list, ind, value);
}

int AddToList(products_list_t *list, void *value) {
    NotNullListRequired(list, "InsertAt");

    int type_ind = indexOf_state(&list->state, ((product_t *) value)->product_type);
    Require(type_ind != -1, "ProductType"Exception, "InsertAt","product type not found");

    if (list->count == list->arrayCount)
        resizeList(list, list->arrayCount * 2);
    list->count++;

    //new
    if (updateStateAdd(&list->state, (*(product_t *) value).product_type)){
        list->count--;
        return 1;
    }
    //end new
    list->content[list->count - 1] = *(product_t *) value;
    return 0;
}

int cmp_types_products(product_t *product1, const char *type) {
    return cmp_types(product1->product_type, type);
}

//return value --> cuanto falto para suplir la cantidad especificada
//                 en "amount_to_extract".
int DeleteOfType(products_list_t *list, int amount_to_extract, char *type) {
    NotNullListRequired(list, "DeleteOfType");
    NotNullRequired(type, "DeleteOfType", "type its Null");
    Require(amount_to_extract > 0, StateListException, "DeleteOfType", "amount_to_extract its less equals than 0");


    int type_ind = indexOf_state(&list->state, type);
    printf("type_ind = %d\n", type_ind);

    if (type_ind < 0)
        return amount_to_extract;

    int current_amount = list->state.amounts[type_ind].amount - amount_to_extract;
    printf("current_amount = %d\n", current_amount);

    if (current_amount >= 0) {
        // list->state.total_amount = list->state.total_amount - amount_to_extract;
        // list->state.amounts[type_ind].amount = current_amount;
        for(int i = 0; i < list->count && amount_to_extract; ++i) {
            if(!cmp_types(list->content[i].product_type, type)) {
                amount_to_extract--;
                DeleteAt(list,i);
            }
        }
        current_amount = 0;
    } else {
        list->state.total_amount = list->state.total_amount - list->state.amounts[type_ind].amount;
        list->state.amounts[type_ind].amount = 0;
    }
    current_amount = -current_amount;
    return -current_amount;
}

//0->no se pudo 1->si
int DeleteOneOfType(products_list_t *list, char *type) {
    NotNullListRequired(list, "DeleteOneOfType");
    NotNullRequired(type, "DeleteOneOfType", "type its Null");

    int product_ind = IndexOfType(list, type);
    //todo: change it if nodes contains references --> free(products_list->content[index]);
//    Require(product_ind >= 0, ListException, "DeleteOneOfType", "product of specific type not found");

    //new
    if (product_ind < 0 || !updateStateDelete(&list->state, list->content[product_ind].product_type)) {
        return 0;
    }
    //end new

    for (int i = product_ind; i < list->count - 1; ++i)
        list->content[i] = list->content[i + 1];
    list->count--;
    return 1;
}

void *GetAt(products_list_t *list, int ind) {
    NotNullListRequired(list, "GetAt");
    IndexRangeRequired(list, ind, "GetAt");

    return &list->content[ind];
}

//only for products
int DeleteAt(products_list_t *list, int ind) {
    NotNullListRequired(list, "DeleteAt");
    IndexRangeRequired(list, ind, "DeleteAt");

    //new
    if (updateStateDelete(&list->state, list->content[ind].product_type))
        return 1;
    //end new

    //todo: change it if nodes contains references --> free(products_list->content[ind]);
    for (int i = ind; i < list->count - 1; ++i)
        list->content[i] = list->content[i + 1];
    list->count--;
    return 0;
}

//only for products
int AmountOfType(products_list_t *list, char type[3]) {
    NotNullListRequired(list, "AmountOfType");
    NotNullRequired(type, "AmountOfType", "type its Null");

    for (int i = 0; i < list->state.count_different; ++i) {
        if (!cmp_types(list->state.amounts[i].product_types, type))
            return list->state.amounts[i].amount;
    }
    return 0;
}

int IndexOfType(products_list_t *list, char type[3]) {
    NotNullListRequired(list, "IndexOfType");
    NotNullRequired(type, "IndexOfType", "type its Null");

    for (int i = 0; i < list->count; ++i) {
        if (!cmp_types_products(&list->content[i], type))
            return i;
    }
    return -1;
}

int cmp_types(const char *type1, const char *type2) {
    //search the first ind where they are count_different
    int ind = type1[0] != type2[0] ? 0 : type1[1] != type2[1] ? 1 : 2;

    return type1[ind] > type2[ind] ? 1 : type1[ind] < type2[ind] ? -1 : 0;
}
