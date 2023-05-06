#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "../src/common.c"

// Le nombre maximum de caractères qu'un client peut écrire sur le chat.
#define CLIENT_TAILLE_MESSAGE 256
// La taille maximale des arguments d'un message. Par exemple, pour un message privé,
// les arguments sont le nom de l'envoyeur et du destinataire.
#define TAILLE_ARGUMENTS_MESSAGE 64
// La taille des messages envoyés en TCP
#define TCP_TAILLE_MESSAGE CLIENT_TAILLE_MESSAGE + TAILLE_ARGUMENTS_MESSAGE

void gestionnaireErreur(const char *message);
void gestionnaireArguments(int argc, char *argv[]);

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
char *avoirPremierMot(char **curseur, char *message, const int tailleMessage);

