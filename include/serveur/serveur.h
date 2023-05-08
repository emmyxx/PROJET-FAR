#ifndef SERVEUR_H
#define SERVEUR_H

#define NB_ARGS_ATTENDUS 2
#define NB_CLIENTS_EN_ATTENTE 8
#define NB_CLIENTS_MAX 3

#include "../typesMessage.h"
#include "gestionnaireClients.h"

/* -------------------------------------------------------------------------- */
/*                          Mise en place du serveur                          */
/* -------------------------------------------------------------------------- */
int creerSocketEcouteur(int port, int nbClientsEnAttente);

/**
 * @brief Accepte une connexion client entrante sur la socket d'écoute.
 * @param socketEcouteur Le descripteur de la socket d'écoute.
 * @return int Le descripteur de la socket client si la connexion est acceptée avec succès, -1 en cas d'échec.
 */
int accepterClient(const int socketEcouteur);

/* -------------------------------------------------------------------------- */
/*                    Réception et traitement des messages                    */
/* -------------------------------------------------------------------------- */
void *receptionMessages(void *arg);
int routageMessageRecu(client **listeClients, client *clientCourant, void *message);
int controlleurConnexion(const client **listeClients, client *clientCourant, void *message);
int controlleurMessageBroadcast(const client **listeClients, const client *clientCourant, MessageBroadcast messageBroadcast);
int controlleurAttributionPseudo(const client **listeClients, client *clientCourant, AttributionPseudo pseudo);
int controlleurMessagePrive(const client **listeClients, const client *clientCourant, MessagePrive messagePrive);
int envoyerMessageAlerte(const client *clientCourant, char *message, TypeAlerte niveauAlerte);
int broadcast(const client **listeClients, const client *clientCourant, const void *messageFormate);

#endif
