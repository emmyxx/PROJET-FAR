#include <stdlib.h>

void gestionnaireErreur(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}