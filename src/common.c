void gestionnaireErreur(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}

char *avoirPremierMot(char **curseur, char *message, const int tailleMessage)
{
  if (tailleMessage < 2)
  {
    fprintf(stderr, "Le message doit contenir au moins 2 caractères (terminateur '\\0' compris).\n");
    return NULL;
  }

  if (message[0] == '\0')
  {
    *curseur = NULL;
    return NULL;
  }

  char *premierMot = strtok(message, " ");
  *curseur = NULL;

  if (strlen(premierMot) + 1 < tailleMessage)
  {
    *curseur = premierMot + strlen(premierMot) + 1;
  }

  return premierMot;
}
