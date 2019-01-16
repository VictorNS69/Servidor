# Practica Servidor Sistemas Operativos curso 18/19

## Autor

[Víctor Nieves Sánchez](https://twitter.com/VictorNS69)

## Objetivo
Desarrollar una implementación de un sencillo servidor de un protocolo simplificado de tipo FTP, para transferencia de ficheros por medio de sockets.

## Notas

- _lanzador.sh_: Un sencillo script de apoyo que permite arrancar una instancia del servidor y toda una batería de clientes que soliciten transferencia de ficheros. 

## Compilación

$ make

```
cd cliente_src/
cc -Wall -g   -c cliente.c 
cc -Wall -g   -I. -o ../cliente cliente.o 
cd ../
cc -Wall -g   -c servidor.c
cc -Wall -g   -I. -o servidor servidor.o 

```
## Funcionamiento
El servidor se encuentra inicialmente escuchando mensajes por un puerto UDP. Los mensajes representan solicitudes de transferencia de ficheros. Las transferencias, en esta versión simplificada del servidor FTP siempre se harán del servidor hacia el cliente. El cliente transmite un mensaje en el cual indica la operación a realizar y el nombre del fichero original (en el servidor) y el nombre local (en el cliente).

El servidor responde con la misma estructura de mensaje (vía UDP también) indicando si se puede realizar o no la operación (dependiendo de que el fichero original exista o no) y, en el caso de ser posible la transferencia, el puerto TCP que se va a usar.

La transmisión del fichero se realiza a través de conexiones stream por medio de TCP.
