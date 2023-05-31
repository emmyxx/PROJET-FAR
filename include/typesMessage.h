/* -------------------------------------------------------------------------- */
/*           Structures de données des différents types de messages           */
/*           que les clients ou le serveur peuvent s'envoyer.                 */
/* -------------------------------------------------------------------------- */

#ifndef TYPES_MESSAGE_H
#define TYPES_MESSAGE_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>


#include "constantes.h"

typedef enum {
  MESSAGE_BROADCAST,
  MESSAGE_PRIVE,
  PSEUDO,
  MESSAGE_ALERTE,
  MORCEAU_FICHIER,
  FICHIER,
  SALON
} TypeMessage;

typedef struct {
  TypeMessage typeMessage;
  char expediteur[TAILLE_PSEUDO];
  char message[TAILLE_SAISIE_CLIENT];
  char nomSalon[TAILLE_NOM_SALON];
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
  size_t tailleMorceau;
  char nomFichier[NAME_MAX];
  char donnees[TAILLE_MORCEAU_FICHIER];
  bool estDernierMorceau;
} MorceauFichier;

typedef struct {
  TypeMessage typeMessage;
  char nomFichier[NAME_MAX];
  size_t tailleFichier;
} Fichier;

typedef struct {
  TypeMessage typeMessage;
  int idSalon;
} Salon;

#endif
