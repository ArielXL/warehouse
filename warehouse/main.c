#include "../warehouse.h"
#include "main.h"
#include "../conection/connection.h"
#include "../entry_parser.h"
#include "../debuging/debug.h"
#include "products.h"

sem_t list_mutex;

void *ProducerLogic(void *args);

void *ConsumerLogic(void *args);

void SendResp(int fd, int resp);

/* entry */
int main(int argc, char *argv[]) {
    /*declarations*/
    SA_IN client_addr;
    pthread_t tid;
    params_t params;
    socklen_t s_size = sizeof(client_addr);
    products_list_t *warehouse_products = malloc(sizeof(products_list_t) + 4);

    /*assigns and parse*/
    char *buff = malloc(sizeof(char) * 8 + 4);
    int listen_fd = OpenListenFD(argv[1]);
    parse_amounts(argc, argv, warehouse_products);
    sem_init(&list_mutex, 0, 1);

    /*product initialize*/
    bzero(&params, sizeof(params_t));
    params.products_info = warehouse_products;

    WarehouseLog("%s%s%d", "Waiting for connections.", "FD: ", listen_fd);

    /*main loop*/
    while (1) {
        int connect_fd = AcceptConnection(&client_addr, buff, listen_fd);
        params.connection_fd = connect_fd;

        bzero(&tid, sizeof(tid));

        if (!strncmp(buff, "producer", 8)) {
            /*producer logic*/
            pthread_create(&tid, NULL, ProducerLogic, &params);
        } else if (!strncmp(buff, "consumer", 8)) {
            /*consumer logic*/
            pthread_create(&tid, NULL, ConsumerLogic, &params);
        }

    }
    /*finalize(never)*/
    close(listen_fd);
    ClearList(warehouse_products);
    free(warehouse_products);
    free(buff);
}

void *ProducerLogic(void *args) {
    params_t *params = (params_t *) args;
    int conn_fd = params->connection_fd;
    products_list_t *products_info = params->products_info;

    long rw_len = 0;
    int spaces_needed = 0, len_types = 0, len_products = 0;
    product_t *products_in = NULL;
    type_amount_t *amounts_in = NULL;

    pthread_detach(pthread_self());

    WarehouseLog("%s", "New producer connected.");

    while (1) {
        /*read -> len of products*/
        while ((rw_len = read(conn_fd, &len_products, sizeof(int))) < sizeof(int));

        // WarehouseLog("%s", "len received.");

        /*read -> count of types*/
        while ((rw_len = read(conn_fd, &len_types, sizeof(int))) < sizeof(int));

        // WarehouseLog("%s", "count_types received.");

        products_in = products_in ? products_in : malloc(sizeof(product_t) * len_products + 4);
        /*read -> products incoming*/
        while ((rw_len = read(conn_fd, products_in, sizeof(product_t) * len_products)) <
               sizeof(product_t) * len_products);

        // WarehouseLog("%s", "products received.");

        amounts_in = amounts_in ? amounts_in : malloc(sizeof(type_amount_t) * len_types + 4);
        /*read -> amounts by type*/
        while ((rw_len = read(conn_fd, amounts_in, sizeof(type_amount_t) * len_types)) <
               sizeof(type_amount_t) * len_types);

        // WarehouseLog("%s", "types amount received.");

        sem_wait(&list_mutex);
        //async
        spaces_needed = UpdateIncoming(products_info, products_in, amounts_in, len_types, len_products);
        sem_post(&list_mutex);

        if (spaces_needed > 0) {
            WarehouseLog("%s", "Cannot receive the amount specified.");
            /* waiting up to free some spaces */

            while (spaces_needed != 0) {
                SendResp(conn_fd, NoUpdated);
                sleep(1);

                sem_wait(&list_mutex);
                //async
                spaces_needed = UpdateIncoming(products_info, products_in, amounts_in, len_types, len_products);
                sem_post(&list_mutex);
            }
        }
        SendResp(conn_fd, Updated);

        WarehouseLog("%s", "products received.");

        sem_wait(&list_mutex);
        //async
        PrintState(products_info, 0, 0);

        sem_post(&list_mutex);

        sleep(1);
    }
}

void *ConsumerLogic(void *args) {

    params_t *params = (params_t *) args;
    int conn_fd = params->connection_fd;
    products_list_t *products_info = params->products_info;

    long rw_len = 0;
    int spaces_needed = 0, len_types = 0, len_products = 0;
    product_t *products_out = NULL;
    type_amount_t *amounts_out = NULL;

    pthread_detach(pthread_self());

    WarehouseLog("%s", "New consumer connected.");

    while (1) {
        /*read -> len of products*/
        while ((rw_len = read(conn_fd, &len_products, sizeof(int))) < sizeof(int));

        // WarehouseLog("%s", "length asked.");

        /*read -> len of types*/
        while ((rw_len = read(conn_fd, &len_types, sizeof(int))) < sizeof(int));

        // WarehouseLog("%s", "count_types asked.");

        amounts_out = amounts_out ? amounts_out : malloc(sizeof(type_amount_t) * len_types + 4);
        /*read -> amounts by type*/
        while ((rw_len = read(conn_fd, amounts_out, sizeof(type_amount_t) * len_types)) <
               sizeof(type_amount_t) * len_types);

        // WarehouseLog("%s", "types amount asked.");

        sem_wait(&list_mutex);

        products_out = products_out ? products_out : malloc(sizeof(product_t) * len_products + 4);

        int products_needed = UpdateOutlet(products_info, products_out, amounts_out, len_types, len_products);

        sem_post(&list_mutex);

        if (products_needed != 0) {
            
            WarehouseLog("%s", "Cannot send the amount specified.");
            /* waiting up to free some spaces */

            while (products_needed != 0) {
                SendResp(conn_fd, NoUpdated);
                sleep(1);

                sem_wait(&list_mutex);
                products_needed = UpdateOutlet(products_info, products_out, amounts_out, len_types, len_products);
                sem_post(&list_mutex);
            }
        } 
        
        SendResp(conn_fd, Updated);

        /*write -> send products*/
        while ((rw_len = write(conn_fd, products_out, sizeof(product_t) * len_products)) <
                                sizeof(product_t) * len_products);

        WarehouseLog("%s", "products sent.");

        sem_wait(&list_mutex);
        //async
        PrintState(products_info, 0, 0);

        sem_post(&list_mutex);

        sleep(1);
    }
}

void SendResp(int fd, int resp) {
    while (write(fd, &resp, sizeof(int)) < sizeof(int));
}
