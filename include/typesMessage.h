/* -------------------------------------------------------------------------- */
/*           Structures de données des différents types de messages           */
/*           que les clients ou le serveur peuvent s'envoyer.                 */
/* -------------------------------------------------------------------------- */

#ifndef TYPES_MESSAGE_H
#define TYPES_MESSAGE_H

#include <limits.h>
#include <stdio.h>

#include "constantes.h"

typedef enum {
  MESSAGE_BROADCAST,
  MESSAGE_PRIVE,
  PSEUDO,
  MESSAGE_ALERTE,
  INFORMATIONS_FICHIER,
  MORCEAU_FICHIER
} TypeMessage;

typedef struct {
  TypeMessage typeMessage;
  char expediteur[TAILLE_PSEUDO];
  char message[TAILLE_SAISIE_CLIENT];
} MessageBroadcast;

typedef struct {
  TypeMessage typeMessage;
  char pseudo[TAILLE_PSEUDO];
} AttributionPseudo;

typedef struct {
  TypeMessage typeMessage;
  TypeAlerte typeAlerte;
  char message[TAILLE_SAISIE_CLIENT];
} MessageAlerte;

typedef struct {
  TypeMessage typeMessage;
  char expediteur[TAILLE_PSEUDO];
  char destinataire[TAILLE_PSEUDO];
  char message[TAILLE_SAISIE_CLIENT];
} MessagePrive;

typedef struct {
  TypeMessage typeMessage;
  size_t tailleFichier;
  char nomFichier[NAME_MAX];
} InformationsFichier;

typedef struct {
  TypeMessage typeMessage;
  char donnees[TAILLE_MORCEAU_FICHIER];
} MorceauFichier;

#endif
