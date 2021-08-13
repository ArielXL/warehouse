#include "producer_consumer.h"

int get_port(int argc, char **argv) {
    Require(argc > 1, Exception, "get_port", "ip:port not founded");

    int port = 0;
    int i = find_char(argv[argc - 1], ':');

    Require(i != -1, Exception, "get_port", "port not found");

    for (i++; i < strlen(argv[argc - 1]); i++) {
        port *= 10;
        port += argv[argc - 1][i] - '0';
    }
    return port;
}

char *get_hostname(int argc, char **argv) {

    char *hostname = malloc(sizeof(char) * 18 + 4);
    bzero(hostname, sizeof(char) * 18);

    for (int i = 0; i < strlen(argv[argc - 1]) && argv[argc - 1][i] != ':'; i++)
        hostname[i] = argv[argc - 1][i];

    return hostname;
}

void generate_default(type_amount_t *type_amount) {
    NotNullRequired(type_amount, "generate_default", "type_amount its null");

    type_amount[0].type[0] = (char) 65;
    type_amount[0].count = 1;
}

void get_product_type(char *argv, char *let_type) {
    for (int i = 0; i < strlen(argv) && argv[i] != ':'; i++)
        let_type[i] = argv[i];
}

int get_number(char *argv) {

    int number = 0;
    int pos = find_char(argv, ':');

    Require(pos != -1, Exception, "get_number", "type count not found");

    for (int i = pos + 1; i < strlen(argv); i++) {
        number *= 10;
        number += argv[i] - '0';
    }

    return number;
}

void PrintStates(product_t *product, type_amount_t *type_amount, int count_types) {
    fprintf(stdout,
            "\n"COLOR_BLUE"<"COLOR_CLEAR COLOR_NEG COLOR_WHITE_BLUE "STATUS" COLOR_CLEAR COLOR_BLUE">" COLOR_CLEAR":\n");

    printf("(Type, Amount) = \n");
    for (int i = 0; i < count_types; ++i)
        printf("\t\t(%s/%d)\n", type_amount[i].type, type_amount[i].count);

    fprintf(stdout,
            "\n"COLOR_BLUE"<"COLOR_CLEAR COLOR_NEG COLOR_WHITE_BLUE "END-STATUS" COLOR_CLEAR COLOR_BLUE">" COLOR_CLEAR"\n");
}

void ParseTypesAmount(type_amount_t *type_amount, int count_product, int *len_products,
                          char **argv, int is_random) {
    NotNullRequired(type_amount, "ParseTypesAmount", "product its null");
    Require(count_product > 0, Exception, "ParseTypesAmount", "count_product its less or equals than 0");

    if (is_random)
        generate_default(type_amount);
    else {
        for (int i = 1, c = 0; c < count_product; i++, c++) {
            get_product_type(argv[i], type_amount[c].type);
            type_amount[c].count = get_number(argv[i]);
            *len_products += type_amount[c].count;
        }
    }
}

int OpenClientFD(char *hostname, int port) {
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    Require((clientfd = socket(AF_INET, SOCK_STREAM, 0)) >= 0, "Socket"Exception, "OpenClientFD", "");

    Require((hp = gethostbyname(hostname)) != NULL, "GetHostByName"Exception, "OpenClientFD", "");

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy(hp->h_addr_list[0], (char *) &serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons((uint16_t) port);

    Require(connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) >= 0 || close(clientfd), "Connect"Exception,
            "OpenClientFD", "");

    return clientfd;
}

int EstablishConnection(char *hostname, int port) {
    NotNullRequired(hostname, "EstablishConnection", "Hostname its null");

    int clientfd;
    Require((clientfd = OpenClientFD(hostname, port)) > 0, Exception, "EstablishConnection", "OpenClientFD error.");
    return clientfd;
}

void FillProducts(product_t *products, type_amount_t *amounts, int len_products, int len_types) {
    NotNullRequired(products, "FillProducts", "products its null");
    NotNullRequired(amounts, "FillProducts", "amounts its null");
    Require(len_products > 0, Exception, "FillProducts", "len_products its less or equals than 0");
    Require(len_types > 0, Exception, "FillProducts", "len_types its less or equals than 0");

    int id = 1;
    int pos = 0;
    for (int type_index = 0; type_index < len_types; type_index++) {
        for (int i = 0; i < amounts[type_index].count; ++i,++pos,++id) {
            products[pos].product_id = id;
            strncpy(products[pos].product_type, amounts[type_index].type, 3);
        }
    }

}