#ifndef SERVEUR_H
#define SERVEUR_H

#include "../typesMessage.h"
#include "gestionnaireClients.h"

#define NB_CLIENTS_EN_ATTENTE 100
#define NB_CLIENTS_MAX 100
#define CHEMIN_DOSSIER_FICHIERS_SERVEUR "./repertoire_serveur/"

typedef struct argsThread {
  client *client;
  client **clients;
  char listeSalons[NB_SALONS_MAX][NB_CLIENTS_MAX];
} argsThread;

/**
 * @brief Créer le socket qui permettra d'écouter les connexions entrantes.
 * @param port Le port sur lequel le serveur va écouter les connexions entrantes.
 * @return int Le descripteur de la socket d'écoute si la création est réussie,
 * -1 en cas d'échec.
 */
int creerSocketEcoute(int port);

/**
 * @brief Accepte une connexion client entrante sur la socket d'écoute.
 * @param socketEcoute Le descripteur de la socket d'écoute.
 * @return int Le descripteur de la socket client si la connexion est acceptée avec succès,
 * -1 en cas d'échec.
 */
int accepterClient(const int socketEcoute);

/**
 * @brief Traite les messages reçus d'un client.
 * @param arg Struct ArgsThread contenant le client et la liste des clients.
 * @return void* NULL.
 */
void *threadTraitementMessagesClient(void *arg);

/**
 * @brief Traite le premier message reçu d'un client (son pseudo).
 * @note Déconnecte le client si le pseudo est déjà utilisé.
 */
int recevoirPseudo(const client **listeClients, client *clientCourant, void *message);

void *gestionnaireThreadMessage();
void *gestionnaireThreadFichier();

void *threadTraitementFichiersClient(void *arg);

int controlleurFichier(const int socketClient, const Fichier fichierClient);

#endif
