//
// Created by lag on 28/12/18.
//

#include "entry_parser.h"
#include "debuging/debug.h"
#include "product_list/list_errors.h"
#include <stdio.h>

int parse_port(const char *string, long len) {
    int par_port = 0;
    for (int i = 0; i < len; i++, par_port *= 10)
        par_port += string[i] - 48;
    return par_port / 10;
}

// si esta retorna la posicion y si no -1
int find_char(char *argv, char c) {
    for (int i = 0; i < strlen(argv); i++) {
        if (argv[i] == c)
            return i;
    }
    return -1;
}

int contains_char(char *s, char c) {
    unsigned long len = strlen(s);
    int i;
    for (i = 0; i < len && s[i] != c; i++);
    return s[i] is c;
}

int parse_int(const char *string, long len, long start_index) {
    int number = 0;
    for (long i = start_index; i < len; ++i) {
        number *= 10;
        number += string[i] - 48;
    }
    return number;
}

void setTypeInfo(products_list_t *list, char *raw_data) {
    NotNullListRequired(list, "setTypeInfo");
    char *type = raw_data;
    char *amount;
    int contain_type = 0;

    for (amount = raw_data; *amount != ':' && *amount != '\0'; amount++);//(*amount == ':')--> always
    if (*amount == ':') {
        *amount = '\0';
        amount++;
        contain_type = 1;
    }
    printf("type = %s   count = %s ", raw_data, amount);

    Require((strlen(raw_data) > 0 && strlen(raw_data) <= 3 && contain_type || !contain_type), "Parse"Exception, "setTypeInfo",
            " \"length of type string\" type greater than 3 or lesser than 0.");

    CreateTypeSpace(&list->state, type, parse_int(amount, strlen(amount), 0));
}

void parse_amounts(int argc, char **argv, products_list_t *list) {
    InitList(list, 5, sizeof(product_t));
    for (int i = 2; i < argc; i++) {
        if (!contains_char(argv[i], ':'))
            SetTotalCapacity(list, parse_int(argv[i], strlen(argv[i]), 0));
        else
            setTypeInfo(list, argv[i]);
    }
    if (!list->state.total_capacity)
        SetTotalCapacity(list, 0);
}
