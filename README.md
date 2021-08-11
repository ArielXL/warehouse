# Autores

|   **Nombre(s) y Apellidos**   | **Grupo** |              **Correo**              |
| :---------------------------: | :-------: | :----------------------------------: |
| Leonel Alejandro García López |   C-312   |  l.garcia@estudiantes.matcom.uh.cu   |
|     Ariel Plasencia Díaz      |   C-312   | a.plasencia@estudiantes.matcom.uh.cu |

Orientación del Proyecto
========================

El objetivo de este proyecto es simular el problema del "productor-consumidor". La tarea en cuestión consiste en crear un conjunto de almacenes, proveedores y clientes y realizar una simulación con ello. Para eso se tendrán en cuenta las siguientes consideraciones:

Cada equipo implementará 3 programas: un *almacén*, un *productor*, y un *consumidor*.

Sobre el almacén
----------------

* El almacén es un programa (servidor) que escucha por un puerto y permite conexiones de varios clientes. (3pt)

* El *almacén* tiene una capacidad limitada de objetos que puede tener en un momento dado en su inventario. (3pt)

~~~bash
$ gcc warehouse.c -o warehouse
$ ./warehouse 3000 50 # port=3000 capacity=50
~~~

* Cuando el servidor recibe una nueva conexión lo primero que recibirá será un string "producer" o un "consumer". (3pt)

* El almacén debe poder que varios "productores" se conecten y también varios "consumidores". (3pt)

* El almacén cada vez que reciba un producto nuevo, o cada vez que un consumidor consuma un producto, debe visualizar en la pantalla un estado actual del almacén (i.e. Cantidad total de productos, cuantos productos de cada tipo, capacidad disponible. etc.) (3pt)

* El almacén puede además tener un limite máximo para cada tipo de producto que se puede generar. Esto será especificado en el momento de la creación del almacén a través de los parámetros (4pt)

~~~bash
$ gcc warehouse.c -o warehouse
$ ./warehouse 3000 50          # esto significa que puede almacenar un máximo de 50 elementos (de cualquier tipo)
$ ./warehouse 3000 A:20 B:30   # un máximo de 20 productos de tipo "A" y un máximo de 30 de tipo "B"
$ ./warehouse 3000 50 A:20     # máximo total es 50, pero solo 20 de tipo "A" (de tipo B pueden ser 50)
~~~

* La parte relacionada con la sincronización a la hora de escribir en la estructura de datos que se implemente en el almacén será utilizando hilos. Y también la parte de la espera cuando el buffer esté lleno (a la hora de escribir) o cuando esté vacío (a la hora de leer) (5pt)

* Se pueden tener más de un almacén ejecutándose simultáneamente (6pt)

Sobre el proveedor
-------------------

* El *productor* es un programa que genera nuevos productos y que se conecta al almacén para que este los guarde. (3pt)

* El programa productor en el momento que comienza la conexión envia el string "producer" hacia el almacén para identificarse (3pt)

* Cada equipo debe implementar un productor simple que genera un producto *cada 1 segundo* y lo envia hacia el almacén (3pt)

* Si el almacén no puede aceptar nuevos productos (porque está lleno) el productor debe parar la producción. (3pt)

* El productor debe imprimir por la consola los productos que va generando (en el momento que los genera) (3pt)

* Cada productor tendrá un identificador (de 3 caracteres) que formará parte de todos los productos que genera. (3pt)

* Cada producto generado tambien tendra un identificador que será un número entero secuencial (i.e. el primero producto es el 1, el segundo es el 2 y asi sucesivamente) (3pt)

* En el momento de la ejecución el productor recibe como parámetro el IP y puerto donde está el almacén de forma ip:puerto (3pt)

~~~bash
$ gcc producer.c -o producer
$ ./producer 10.6.122.44:3000
~~~

* El productor puede producir más de un tipo de producto. Esta información se especificará a la hora de la creación de un productor como parte de los parámetros. (4pt)

* Además de los tipos de productos que se pueden generar, también recibirá como parámetro un número entero que significa la cantidad de productos de ese tipo que se generan por intervalo de tiempo. (cada 1 segundo) (4pt)

~~~bash
$ gcc producer.c -o producer
$ ./producer A:1 B:2 10.6.122.44:3000 # un producto de tipo "A" y dos producto de tipo "B" cada 1 segundo
~~~

* El productor puede conectarse a más de un almacén y producir hacia todos los almacenes simultáneamente. (6pt)

* Los requerimientos anteriores se mantienen, asi que si un productor genera un producto por segundo, y está conectado a 2 almacenes, entonces como máximo, cada almacén recibirá un producto cada 2 segundos. (6pt)

* El productor recibirá como parámetros todos los almacenes a los que el se va a conectar. (6pt)

~~~bash
$ gcc producer.c -o producer
$ ./producer A:1 B:2 10.6.122.44:3000 10.6.122.44:4000 10.6.122.45:3500 # 3 almacenes
~~~

* Cada equipo de estudiantes decidirá la estrategia para repartir los productos entre los diferentes almacenes (6pt)

Sobre el consumidor
-------------------

* El *consumidor* es un programa que consume productos y que se conecta al almacén para que este se los provea. (3pt)

* El programa consumidor en el momento que comienza la conexión envia el string "consumer" hacia el almacén para identificarse (3pt)

* Cada equipo debe implementar un consumidor simple que consume un producto *cada 1 segundo* desde el almacén (3pt)

* Si el almacén no puede generar productos (porque está vacío) el consumidor debe quedarse esperando a que haya disponibilidad. (3pt)

* El consumidor debe imprimir por la consola los productos que va consumiendo (en el momento que los consume) (3pt)

* En el momento de la ejecución el consumidor recibe como parámetro el IP y puerto donde está el almacén de forma ip:puerto (3pt)

~~~bash
$ gcc consumer.c -o consumer
$ ./consumer 10.6.122.44:3000
~~~

* El consumidor puede especificar la cantidad de productos que necesita en cada intervalo de tiempo y además debe poder especificar los tipos de productos que necesita (4pt)

~~~bash
$ gcc consumer.c -o consumer
$ ./consumer A:2 B:3 10.6.122.44:3000 # cada 1 segundo este consumidos necesita 2 productos de tipo "A" y 3 productos de tipo "B"
~~~

* Cada programa consumidor puede conectarse a más de un almacén (6pt)

* En la terminal, en el momento de la creación, se le pasará como parámetro todos los ip:puerto de todos los almacenes a los que el se conectará. (6pt)

~~~bash
$ gcc consumer.c -o consumer
$ ./consumer A:2 B:3 10.6.122.44:3000 10.6.122.45:4000 # 2 almacenes
~~~

* Cada equipo decidirá la mejor estrategia para consumir productos desde diferentes almacenes (6pt)

Sobre los productos
-------------------

* El producto será una instancia de la siguiente estructura de c

~~~c
struct product {
    char provider_id[3];      // identificador del productor
    int  product_id;          // numero secuencial del producto
    char product_type[3];     // tipo de producto generado
    char product_data[4084];  // esto es solo para que todos los productos tengan 4096 bytes de tamaño
}
~~~

Nota: Para coger 6 se debe cumplir con todas las especificaciones señaladas con esta nota, en caso de no completarse o que no funcionen algunas de ellas, podrá obtener notas entre 4 y 5.5, teniendo en cuenta que todo lo demás funciona correctamente.
