#ifndef TYPES_MESSAGE_H
#define TYPES_MESSAGE_H

#include "constantes.h"

typedef enum
{
    MESSAGE_BROADCAST,
    PSEUDO,
    MESSAGE_ALERTE,
    MESSAGE_SERVEUR,
    MESSAGE_PRIVE,
} TypeMessage;

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

#endif
