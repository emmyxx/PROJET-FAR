#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "../src/common.c"

#define TAILLE_MESSAGE 256

void gestionnaireErreur(const char *message);
void gestionnaireArguments(int argc, char *argv[]);
