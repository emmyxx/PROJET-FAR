#include <stdlib.h>
#include <stdio.h>

#define TAILLE_MESSAGE 256

void gestionnaireErreur(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}