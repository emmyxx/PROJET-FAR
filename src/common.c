#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define TAILLE_MESSAGE 256

void gestionnaireErreur(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}