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

// Le nombre maximum de caractères qu'un client peut écrire sur le chat.
#define TAILLE_SAISIE_CLIENT 1024
// La taille maximale des arguments d'un message. Par exemple, pour un message privé,
// les arguments sont le type du message, le nom de l'envoyeur et du destinataire.
// La somme de leur taille ne doit pas dépasser TAILLE_ENTETE.
#define TAILLE_ENTETE 256
// La taille des messages envoyés en TCP
#define TAILLE_MESSAGE_TCP TAILLE_SAISIE_CLIENT + TAILLE_ENTETE
#define TAILLE_PSEUDO 20

typedef enum
{
    MESSAGE_BROADCAST,
    PSEUDO,
    MESSAGE_ALERTE,
    MESSAGE_SERVEUR,
    MESSAGE_PRIVE,
} TypeMessage;

typedef enum
{
    INFORMATION,
    AVERTISSEMENT,
    ERREUR,
} TypeAlerte;

typedef struct
{
    TypeMessage typeMessage;
    char expediteur[TAILLE_PSEUDO];
    char message[TAILLE_SAISIE_CLIENT];
} MessageBroadcast;

typedef struct
{
    TypeMessage typeMessage;
    char pseudo[TAILLE_PSEUDO];
} AttributionPseudo;

typedef struct
{
    TypeMessage typeMessage;
    TypeAlerte typeAlerte;
    char message[TAILLE_SAISIE_CLIENT];
} MessageAlerte;

typedef struct
{
    TypeMessage typeMessage;
    char expediteur[TAILLE_PSEUDO];
    char destinataire[TAILLE_PSEUDO];
    char message[TAILLE_SAISIE_CLIENT];
} MessagePrive;

void gestionnaireErreur(const char *message);
void gestionnaireArguments(int argc, char *argv[]);

#endif