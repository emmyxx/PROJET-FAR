#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>

#include "constantes.h"

void gestionnaireErreur(const char *message);
void gestionnaireArguments(int argc, char *argv[]);

#endif