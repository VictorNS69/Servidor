# Practica Servidor Sistemas Operativos curso 18/19

## Autor

[Víctor Nieves Sánchez](https://twitter.com/VictorNS69)


## Notas

- _lanzador.sh_: Un sencillo script de apoyo que permite arrancar una instancia del servidor y toda una batería de clientes que soliciten transferencia de ficheros. 

## Compilación

$ make

```
cc -Wall -g   -I. -o ../cliente cliente.o 
cc -Wall -g   -I. -o servidor servidor.o 
```

