#include "../producer_consumer.h"

int main(int argc, char **argv) {
    int port = get_port(argc, argv);
    char *hostname = get_hostname(argc, argv);

    int count_types = argc - 2;
    int len_products = 1;
    int is_random = 0;
    int response;

    if(!count_types) {
        is_random = 1;
        count_types = 1;
    }
    len_products = is_random;

    type_amount_t *type_amount = malloc(sizeof(type_amount_t) * count_types + 4);
    bzero(type_amount, sizeof(type_amount_t) * count_types);

    ParseTypesAmount(type_amount, count_types, &len_products, argv, is_random);

    product_t *product_out = malloc(sizeof(product_t) * len_products + 4);
    bzero(product_out, sizeof(product_t) * len_products + 1);

    FillProducts(product_out, type_amount, len_products, count_types);

    PrintStates(product_out, type_amount, count_types);

    int fd = EstablishConnection(hostname, port);

    ProducerLog("%s%s", "Producer connected to host", hostname);

    char *buffer = malloc(sizeof(char) * 8 + 4);
    bzero(buffer, sizeof(char) * 8 + 1);
    strcpy(buffer, "producer");

    /*write -> producer*/
    while (write(fd, buffer, sizeof(char) * 8) < sizeof(char) * 8);

    free(buffer);

    while (1) {
        /*write -> len of products*/
        while (write(fd, &len_products, sizeof(int)) < sizeof(int));

        /*write -> count of types*/
        while (write(fd, &count_types, sizeof(int)) < sizeof(int));

        /*write -> products*/
        while (write(fd, product_out, sizeof(product_t) * len_products) < sizeof(product_t) * len_products);

        /*write -> amounts by type*/
        while (write(fd, type_amount, sizeof(type_amount_t) * count_types) < sizeof(type_amount_t) * count_types);

        response = NoUpdated;

        /*wait for a positive response*/
        while (response is NoUpdated)
            while (read(fd, &response, sizeof(int)) < sizeof(int));

        ProducerLog("%d%s", len_products, "products has been sent.");

        sleep(1);
    }
}