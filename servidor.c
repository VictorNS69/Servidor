/* Autor: Victor Nieves Sanchez
* servidor: servidor pseudo FTP.
*/

//includes enunciado
#include <sys/types.h>
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
  int sTCP, sUDP, pTCP;
  struct sockaddr_in sin;
  socklen_t size = sizeof(sin);
  int msize, file, cs;
  size_t len;
  ssize_t result;
  UDP_Msg msg;
  char buffer[1024];

  /* Creacion del socket UDP */
  fprintf(stdout,"SERVIDOR: Creacion del socket UDP: ");
  if ((sUDP = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stdout, "ERROR\n");
    exit(1);
  }
  fprintf(stdout, "OK\n");

  /* Asignacion de la direccion local (del servidor) Puerto UDP*/
  bzero((char *) &sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(HOST_SERVIDOR);
  sin.sin_port = htons(0);
  fprintf(stdout, "SERVIDOR: Asignacion del puerto servidor: ");
  if (bind(sUDP, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
    fprintf(stdout, "ERROR\n");
    close(sUDP);
    exit(1);
  }
  fprintf(stdout, "OK\n");

  /* Escribimos el puerto de servicio */
  bzero((char *) &sin, sizeof(struct sockaddr_in));//reset
  getsockname(sUDP, (struct sockaddr *) &sin, &size);
  Escribir_Puerto(ntohs((int) sin.sin_port));

  /* Creacion del socket TCP de servicio */
  fprintf(stdout,"SERVIDOR: Creacion del socket TCP: ");
  if ((sTCP = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stdout, "ERROR\n");
    exit(1);
  }
  fprintf(stdout, "OK\n");

  /* Asignacion de la direccion local (del servidor) Puerto TCP*/
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(HOST_SERVIDOR);
  sin.sin_port = htons(0);
  fprintf(stdout,"SERVIDOR: Asignacion del puerto servidor: ");
  if (bind(sTCP, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
    fprintf(stdout, "ERROR\n");
    close(sTCP);
    exit(1);
  }
  fprintf(stdout, "OK\n");
  bzero((char *) &sin, sizeof(struct sockaddr_in)); /* Pone a 0*/
  getsockname(sTCP, (struct sockaddr *) &sin, &size);
  pTCP = htons(sin.sin_port);

  /* Aceptamos conexiones por el socket */
  fprintf(stdout,"SERVIDOR: Aceptacion de peticiones: ");
  if (listen(sTCP, 10) < 0) {
    fprintf(stdout, "ERROR\n");
    fprintf(stdout, "SERVIDOR: Puerto TCP reservado: ERROR\n");
    exit(1);
  }
  fprintf(stdout, "OK\n");

  /* Puerto TCP ya disponible */
  fprintf(stdout,"SERVIDOR: Puerto TCP reservado: OK\n");
  while(1 /* Bucle de procesar peticiones */){
    fprintf(stdout,"SERVIDOR: Esperando mensaje.\n");
    bzero((char *) &sin, sizeof(struct sockaddr_in));
    size = sizeof(sin);
    bzero(&msg, sizeof(UDP_Msg));
    msize = sizeof(UDP_Msg);

    /* Recibo mensaje */
    fprintf(stdout, "SERVIDOR: Mensaje del cliente: ");
    if (recvfrom(sUDP, (char *) &msg, (size_t) msize, 0, (struct sockaddr *) &sin, &size) < 0) {
      fprintf(stdout, "ERROR\n");
      break;
    }
    fprintf(stdout, "OK\n");
    if(ntohl(msg.op) == QUIT/* msg QUIT*/){
      fprintf(stdout,"SERVIDOR: QUIT\n");
      msg.op = htonl(OK);
      fprintf(stdout,"SERVIDOR: Enviando del resultado [OK]: ");
      result = sendto(sUDP, (char *) &msg, (size_t) msize, 0, (struct sockaddr *) &sin, size);
      if (result < 0)
        fprintf(stdout, "ERROR\n");
      else
        fprintf(stdout, "OK\n");
      break;
    }
    else if (ntohl(msg.op) == REQUEST) {
      fprintf(stdout,"SERVIDOR: REQUEST(%s,%s)\n", msg.local, msg.remoto);

      /* Envio del resultado */
      file = open(msg.remoto, O_RDONLY);
      if (file < 0) {
        fprintf(stdout,"SERVIDOR: Enviando del resultado [ERROR]: ");
        msg.op = htonl(ERROR);
      }
      else {
        fprintf(stdout,"SERVIDOR: Enviando del resultado [OK]: ");
        msg.op = htonl(OK);
        msg.puerto = htons(pTCP);
      }
      result = sendto(sUDP, (char *) &msg, (size_t) msize, 0, (struct sockaddr *) &sin, size);
      if (result < 0)
        fprintf(stdout, "ERROR\n");
      else
        fprintf(stdout, "OK\n");

      /* Esperamos la llegada de una conexion */
      fprintf(stdout,"SERVIDOR: Llegada de un mensaje: ");
      if (msg.op == htonl(OK)) {
        cs = accept(sTCP, (struct sockaddr *) &sin, &size);
        if (cs < 0)
          fprintf(stdout, "ERROR\n");
        else
          fprintf(stdout, "OK\n");
        while ((len = (size_t) read(file, buffer, 1024)) > 0)
          write(cs, buffer, len);
        close(cs);
      }
      close(file);
    }
  }
  //Cerramos todo lo que quede por cerrar
  close(sTCP);
  close(sUDP);
  fprintf(stdout,"SERVIDOR: Finalizado\n");
  exit(0);
}

/* Funcion auxiliar que escribe un numero de puerto en el file */
void Escribir_Puerto (int puerto){
  int fd;
  if((fd=creat(FICHERO_PUERTO,0660))>=0){
    write(fd,&puerto,sizeof(int));
    close(fd);
    fprintf(stdout,"SERVIDOR: Puerto guardado en fichero %s: OK\n",FICHERO_PUERTO);
  }
}
