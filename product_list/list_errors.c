//
// Created by lag on 30/12/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_errors.h"
#include "../utils_defines.h"

//requires

void Require(int bool, const char *exception, const char *methodName, const char *message) {
    if (!bool)
        ThrowException(exception, methodName, message);
}

void NotNullListRequired(products_list_t *object, const char *methodName) {
    NotNullRequired(object, methodName, "products_list its not declared or its null");
    NotNullRequired(object->content, methodName, "products_list->content its null");
}

void NotNullRequired(void *object, const char *methodName, const char *message) {
    if (object == NULL)
        ThrowException(NullReferenceException, methodName, message);
}

void IndexRangeRequired(products_list_t *list, int index, const char *methodName) {
    if (index < 0 || index >= list->count)
        ThrowIndexOutOfRangeException(list, index, methodName);
}

//throws
void ThrowException(const char *exception, const char *methodName, const char *message) {
//    WarehouseLog("%s%s%s%s%s", exception, ":  method=(", methodName, ") - ", message);
    fprintf(stderr, COLOR_RED "[%s]"COLOR_CLEAR":  method=(%s) - %s\n", exception, methodName, message);
    exit(EXIT_FAILURE);
}

void ThrowIndexOutOfRangeException(products_list_t *list, int index, const char *methodName) {
    fprintf(stderr, COLOR_RED "[%s]"COLOR_CLEAR": method=(%s) - products_list.count_different=%d, Index=%d\n", IndexOutOfRangeException,
            methodName, list->count, index);
    exit(EXIT_FAILURE);
}
