//
// Created by lag on 30/12/18.
//

#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <stdlib.h>

#define not !
#define or ||
#define and &&
#define is ==
#define not_is !=

#define ThereArePlace(p_state, type_ind) (type_ind >= 0 && ((p_state->amounts[type_ind].capacity > 0 && p_state->amounts[type_ind].amount < p_state->amounts[type_ind].capacity) || (p_state->amounts[type_ind].capacity == 0 && p_state->total_amount < p_state->total_capacity)))
#define ThereAreSome(p_state, type_ind) (type_ind >= 0 && p_state->total_amount > 0 && p_state->amounts[type_ind].amount > 0)

#define DecToZero(arg) arg = arg > 0 ? arg - 1 : 0;

typedef struct product {
    char provider_id[3];     // identificador del productor
    int product_id;          // numero secuencial del productor
    char product_type[3];    // tipo de producto generado
    char product_data[4084]; // esto es solo para que todos los productos tengan 4096 bytes de tamaño
} product_t;

typedef struct tuple_amount_type {
    char product_types[3];
    int amount;
    int capacity;        //todo all about capacity
} tuple_amount_type_t;

typedef struct products_state {
    int count_different;
    int arrayCount;
    int restricted;      //0-> admite nuevos tipos --- 1-> no los admite
    int total_amount;   //todo all about amounts
    int total_capacity;
    tuple_amount_type_t *amounts;
} products_state_t;

typedef struct products_list {
    int count;
    int arrayCount;
    size_t sizeType;
    product_t *content;
    products_state_t state;
} products_list_t;


//void setProductAt(products_list_t *products_list, int index, void *value);
//int cmp_types_products(product_t *product1, const char *type);

int indexOf_state(products_state_t *state, char *product_type);

int DeleteOfType(products_list_t *list, int amount_to_extract, char *type);

int SetAt(products_list_t *list, int ind, void *value);

void SetTotalCapacity(products_list_t *list, int total_capacity);

int CreateTypeSpace(products_state_t *state, char *type, int capacity);

void *GetAt(products_list_t *list, int ind);

int AmountOfType(products_list_t *list, char type[3]);

int DeleteAt(products_list_t *list, int ind);

int DeleteOneOfType(products_list_t *list, char type[3]);

int AddToList(products_list_t *list, void *value);

void ClearList(products_list_t *list);

void InitList(products_list_t *list, int initial_len, size_t size_type);

int IndexOfType(products_list_t *list, char type[3]);


#endif //LIST_LIST_H
