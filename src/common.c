void gestionnaireErreur(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}

/**
 * @brief Récupère le premier mot d'une chaîne de caractères.
 * @param curseur Pointeur vers ce qu'il y a après le premier mot.
 * Est NULL si la chaîne de caractères ne contient qu'un mot.
 * @param message Chaîne de caractères contenant le premier mot à extraire.
 * @param tailleMessage Taille du message (terminateur '\0' compris).
 * Doit être strictement supérieure à 1.
 * @return Le pointeur premier mot extrait de la chaîne de caractères ou NULL si
 * si la chaîne commence par un terminateur \0.
 * @warning Le premier " " après le premier mot est remplacé par le terminateur '\0'.
 */
char *avoirPremierMot(char **curseur, char *message, const int tailleMessage)
{
  if (tailleMessage < 2)
  {
    fprintf(stderr, "Le message doit contenir au moins 2 caractères (terminateur '\\0' compris).\n");
    exit(EXIT_FAILURE);
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

