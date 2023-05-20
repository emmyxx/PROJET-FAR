#include "../include/common.h"
#include "../include/constantes.h"


void gestionnaireErreur(const char *message) {
  perror(message);
  exit(EXIT_FAILURE);
}
