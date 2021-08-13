PORT = 9000
ADDR = localhost


build: build_warehouse build_consumer build_producer

build_consumer:
	gcc debuging/** product_list/** consumer/consumer.c entry_parser.c producer_consumer.c -o exec_consumer

build_producer:
	gcc debuging/** product_list/** producer/** entry_parser.c producer_consumer.c -o exec_producer

build_warehouse:
	gcc conection/** debuging/** entry_parser.* producer_consumer.* product_list/** warehouse/** -o exec_warehouse -l pthread

run_warehouse:
	./exec_warehouse $(PORT) 50