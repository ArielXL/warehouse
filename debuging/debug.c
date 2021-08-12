//
// Created by lag on 28/12/18.
//

#include <stdio.h>
#include "debug.h"
#include "../utils_defines.h"
#include "../warehouse.h"
#include "../product_list/list_errors.h"
#include <stdarg.h>
#include <printf.h>


void PrintProd(product_t *p) {
    printf("\tId:%d -- Type:%s\n", p->product_id, p->product_type);
}

void PrintState(products_list_t *list, int num_id, int explicit) {
    fprintf(stdout,
            "\n"COLOR_BLUE"<"COLOR_CLEAR COLOR_NEG COLOR_WHITE_BLUE "STATUS" COLOR_CLEAR COLOR_BLUE">" COLOR_CLEAR":\n");

    printf("Products-Count:%d\n", list->count);
//    printf("SizeType:%ld\n", list->sizeType);
//    printf("ArrayCount:%d\n", list->arrayCount);

    printf("<- - - -state(%d)- - - ->\n", num_id);
//    printf("Restricted: %s\n", list->state.restricted ? "yes" : "no");
//    printf("[Count_different, ArrayCount] = [%d, %d]\n", list->state.count_different, list->state.arrayCount);
    printf("[Total_amount/Total_capacity] = [%d/%d]\n", list->state.total_amount, list->state.total_capacity);
    printf("(Type, Amount) = \n");
    for (int i = 0; i < list->state.count_different; ++i) {
        printf("\t(%s, %d/%d) \n", list->state.amounts[i].product_types, list->state.amounts[i].amount,
               list->state.amounts[i].capacity ? list->state.amounts[i].capacity : list->state.total_capacity);
    }
    printf("<- - - -/state(%d)- - - ->\n", num_id);
    if (explicit) {
        for (int i = 0; i < list->count; ++i)
            PrintProd(&list->content[i]);
    }
    fprintf(stdout,
            "\n"COLOR_BLUE"<"COLOR_CLEAR COLOR_NEG COLOR_WHITE_BLUE "END-STATUS" COLOR_CLEAR COLOR_BLUE">" COLOR_CLEAR":\n");
}


void WarehouseLog(const char *string_format, ...) {
    char *result_str;
    char result_chr;
    int result_int;
    va_list pa;

    va_start(pa, string_format);

    fprintf(stdout,
            COLOR_BLUE "["COLOR_CLEAR COLOR_NEG COLOR_GREEN "Warehouse" COLOR_CLEAR COLOR_BLUE "]" COLOR_CLEAR ": ");

    while (*string_format) {

        if (*string_format == '%') {
            string_format++;

            switch (*string_format) {
                case 's':
                    result_str = va_arg(pa, char*);
                    PRINT_STR(result_str);
                    break;
                case 'c':
                    result_chr = (char)va_arg(pa, int); // ---> insecure
                    PRINT_CHR(result_chr);
                    break;
                case 'd':
                    result_int = va_arg(pa, int);
                    PRINT_INT(result_int);
                    break;
                default:
                    Require(0, "UnexpectedType"Exception, "WarehouseLog",
                            "Passed wrong argument types (different to a char* or int).");

            }
        }
        string_format++;
    }
    printf("\n");

    va_end(pa);
}

void ConsumerLog(const char *string_format, ...) {
    char *result_str;
    char result_chr;
    int result_int;
    va_list pa;

    va_start(pa, string_format);

    fprintf(stdout,
            COLOR_BLUE "["COLOR_CLEAR COLOR_NEG COLOR_GREEN "Consumer" COLOR_CLEAR COLOR_BLUE "]" COLOR_CLEAR ": ");

    while (*string_format) {

        if (*string_format == '%') {
            string_format++;

            switch (*string_format) {
                case 's':
                    result_str = va_arg(pa, char*);
                    PRINT_STR(result_str);
                    break;
                case 'c':
                    result_chr = (char)va_arg(pa, int); // ---> insecure
                    PRINT_CHR(result_chr);
                    break;
                case 'd':
                    result_int = va_arg(pa, int);
                    PRINT_INT(result_int);
                    break;
                default:
                    Require(0, "UnexpectedType"Exception, "WarehouseLog",
                            "Passed wrong argument types (different to a char* or int).");

            }
        }
        string_format++;
    }
    printf("\n");

    va_end(pa);
}

void ProducerLog(const char *string_format, ...) {
    char *result_str;
    char result_chr;
    int result_int;
    va_list pa;

    va_start(pa, string_format);

    fprintf(stdout,
            COLOR_BLUE "["COLOR_CLEAR COLOR_NEG COLOR_GREEN "Producer" COLOR_CLEAR COLOR_BLUE "]" COLOR_CLEAR ": ");

    while (*string_format) {

        if (*string_format == '%') {
            string_format++;

            switch (*string_format) {
                case 's':
                    result_str = va_arg(pa, char*);
                    PRINT_STR(result_str);
                    break;
                case 'c':
                    result_chr = (char)va_arg(pa, int); // ---> insecure
                    PRINT_CHR(result_chr);
                    break;
                case 'd':
                    result_int = va_arg(pa, int);
                    PRINT_INT(result_int);
                    break;
                default:
                    Require(0, "UnexpectedType"Exception, "ProducerLog",
                            "Passed wrong argument types (different to a char* or int).");

            }
        }
        string_format++;
    }
    printf("\n");

    va_end(pa);
}