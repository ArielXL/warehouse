//
// Created by lag on 30/12/18.
//

#include "products_list.h"

#ifndef LIST_LIST_ERRORS_H
#define LIST_LIST_ERRORS_H

#define ListException "Exception"
#define IndexOutOfRangeException "IndexOutOfRangeException"
#define NullReferenceException "NullReferenceException"
#define StateListException "StateListException"
#define Exception "Exception"

//
void IndexRangeRequired(products_list_t *list, int index, const char *methodName);

void NotNullRequired(void *object, const char *methodName, const char *message);

void NotNullListRequired(products_list_t *object, const char *methodName);

void Require(int bool, const char *exception, const char *methodName, const char *message);

//
void ThrowException(const char *exception, const char *methodName, const char *message);

void ThrowIndexOutOfRangeException(products_list_t *list, int index, const char *methodName);


#endif //LIST_LIST_ERRORS_H
