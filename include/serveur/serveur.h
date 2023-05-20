#ifndef SERVEUR_H
#define SERVEUR_H

#define NB_ARGS_ATTENDUS 2
#define NB_CLIENTS_EN_ATTENTE 8
#define NB_CLIENTS_MAX 3

#include "../typesMessage.h"
#include "gestionnaireClients.h"

typedef struct argsThread {
  client *client;
  client **clients;
} argsThread;

/**
 * @brief Créer le socket qui permettra d'écouter les connexions entrantes.
 * @param port Le port sur lequel le serveur va écouter les connexions entrantes.
 * @return int Le descripteur de la socket d'écoute si la création est réussie,
 * -1 en cas d'échec.
 */
int creersocketEcoute(int port);

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

/* -------------------------------------------------------------------------- */
/*                       Réception et envoi des fichiers                      */
/* -------------------------------------------------------------------------- */
void *threadEnvoiFichier(void *arg);
void *threadReceptionFichier(void *arg);

#endif
