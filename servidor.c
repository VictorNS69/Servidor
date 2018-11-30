/* Autor: Victor Nieves Sanchez
 * servidor: servidor pseudo FTP.
 *
 * Copyright (c) 2013,2018 DATSI. Francisco Rosales <frosal@fi.upm.es>
 * Todos los derechos reservados.
 *
 * Publicado bajo Licencia de Proyecto Educativo Práctico
 * <http://laurel.datsi.fi.upm.es/~ssoo/LICENCIA/LPEP>
 *
 * Queda prohibida la difusión total o parcial por cualquier
 * medio del material entregado al alumno para la realización
 * de este proyecto o de cualquier material derivado de este,
 * incluyendo la solución particular que desarrolle el alumno.
 */

//includes enunciado
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "message.h"

//mis includes
#include <arpa/inet.h>
#include <netdb.h>

void Escribir_Puerto (int puerto);

/* FUNCION MAIN DEL PROGRAMA SERVIDOR */
int main(int argc,char* argv[]){

   /* El esqueleto de la funcion principal muestra, de forma esquematica la secuencia
   de operaciones y la correspondiente traza que se sugiere */
   int socketTCP, socketUDP;
   int puertoTCP;
   struct sockaddr_in s_ain;
   socklen_t size = sizeof(s_ain);
   /* Creacion del socket UDP */
   fprintf(stderr,"SERVIDOR: Creacion del socket UDP: ");
   if ((socketUDP = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
       fprintf(stderr, "ERROR\n");
       exit(1);
   }
   fprintf(stderr, "OK\n");

   /* Asignacion de la direccion local (del servidor) Puerto UDP*/
   bzero((char *) &s_ain, sizeof(s_ain));
   s_ain.sin_family = AF_INET;
   s_ain.sin_addr.s_addr = inet_addr(HOST_SERVIDOR);
   s_ain.sin_port = htons(0);
   fprintf(stderr, "SERVIDOR: Asignacion del puerto servidor: ");
   if (bind(socketUDP, (struct sockaddr *) &s_ain, sizeof(s_ain)) < 0) {
       fprintf(stderr, "ERROR\n");
       close(socketUDP);
       exit(1);
   }
   fprintf(stderr, "OK\n");

   /* Escribimos el puerto de servicio */
   bzero((char *) &s_ain, sizeof(struct sockaddr_in)); /* Pone a 0*/
   getsockname(socketUDP, (struct sockaddr *) &s_ain, &size);
   Escribir_Puerto(ntohs((uint16_t) (int) s_ain.sin_port) /* Numero de puerto asignado */);

   /* Creacion del socket TCP de servicio */
   fprintf(stderr,"SERVIDOR: Creacion del socket TCP: ");
   if ((socketTCP = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       fprintf(stderr, "ERROR\n");
       exit(1);
   }
   fprintf(stderr, "OK\n");

   /* Asignacion de la direccion local (del servidor) Puerto TCP*/
   s_ain.sin_family = AF_INET;
   s_ain.sin_addr.s_addr = inet_addr(HOST_SERVIDOR);
   s_ain.sin_port = htons(0);
   fprintf(stderr,"SERVIDOR: Asignacion del puerto servidor: ");
   if (bind(socketTCP, (struct sockaddr *) &s_ain, sizeof(s_ain)) < 0) {
       fprintf(stderr, "ERROR\n");
       close(socketTCP);
       exit(1);
   }
   fprintf(stderr, "OK\n");
   bzero((char *) &s_ain, sizeof(struct sockaddr_in)); /* Pone a 0*/
   getsockname(socketTCP, (struct sockaddr *) &s_ain, &size);
   puertoTCP = htons(s_ain.sin_port);

   /* Aceptamos conexiones por el socket */
   fprintf(stderr,"SERVIDOR: Aceptacion de peticiones: ");
   if (listen(socketTCP, 10) < 0) {
       fprintf(stderr, "ERROR\n");
       fprintf(stderr, "SERVIDOR: Puerto TCP reservado: ERROR\n");
       exit(1);
   }
   fprintf(stderr, "OK\n");

   /* Puerto TCP ya disponible */
   fprintf(stderr,"SERVIDOR: Puerto TCP reservado: OK\n");
   exit(0);
   while(1 /* Bucle de procesar peticiones */)
   {
      fprintf(stderr,"SERVIDOR: Esperando mensaje.\n");

      /* Recibo mensaje */
      fprintf(stderr,"SERVIDOR: Mensaje del cliente: OK|ERROR\n");

      if(1/* Mensaje QUIT*/)
           fprintf(stderr,"SERVIDOR: QUIT\n");
      else
      {
           fprintf(stderr,"SERVIDOR: REQUEST(%s,%s)\n", "local","remoto");

           /* Envio del resultado */
           fprintf(stderr,"SERVIDOR: Enviando del resultado [OK|ERROR]: OK|ERROR\n");
           /* Esperamos la llegada de una conexion */
           fprintf(stderr,"SERVIDOR: Llegada de un mensaje: OK|ERROR\n");
      }
   }

   fprintf(stderr,"SERVIDOR: Finalizado\n");
   exit(0);
}

/* Funcion auxiliar que escribe un numero de puerto en el fichero */
void Escribir_Puerto (int puerto)
{
   int fd;
   if((fd=creat(FICHERO_PUERTO,0660))>=0)
   {
      write(fd,&puerto,sizeof(int));
      close(fd);
      fprintf(stderr,"SERVIDOR: Puerto guardado en fichero %s: OK\n",FICHERO_PUERTO);
   }
}
