#include <pthread.h>
#include <stdbool.h>

#include "../include/constantes.h"
#include "../include/serveur/gestionnaireClients.h"
#include "../include/serveur/routageMessageRecu.h"
#include "../include/serveur/serveur.h"
#include "../include/typesMessage.h"

// Ces fonctions sont déclarées sont statiques (pas accessbiles en dehors du fichier)
// pour cacher l'implémentation de la fonction routageMessageRecu().

/* -------------------------------------------------------------------------- */
/*                                Controlleurs                                */
/* -------------------------------------------------------------------------- */
static int controlleurMessageBroadcast(const client **listeClients, const client *clientCourant,
                                       MessageBroadcast messageBroadcast);
static int controlleurAttributionPseudo(const client **listeClients, client *clientCourant,
                                        AttributionPseudo pseudo);
static int controlleurMessagePrive(const client **listeClients, const client *clientCourant,
                                   MessagePrive messagePrive);
static int controlleurSalon(client *clientCourant, char **listeSalons, Salon salon);

/* -------------------------------------------------------------------------- */
/*                                   Divers                                   */
/* -------------------------------------------------------------------------- */
static int broadcast(const client **listeClients, const client *clientCourant, const void *message);

int routageMessageRecu(client **listeClients, client *clientCourant, void *message,
                       char **listeSalons) {
  const TypeMessage typeMessage = *(TypeMessage *)message;

  if (typeMessage == MESSAGE_BROADCAST) {
    MessageBroadcast messageBroadcast = *(MessageBroadcast *)message;
    return controlleurMessageBroadcast((const client **)listeClients, (const client *)clientCourant,
                                       messageBroadcast);
  }

  if (typeMessage == PSEUDO) {
    AttributionPseudo pseudo = *(AttributionPseudo *)message;
    return controlleurAttributionPseudo((const client **)listeClients, clientCourant, pseudo);
  }

  if (typeMessage == MESSAGE_PRIVE) {
    MessagePrive messagePrive = *(MessagePrive *)message;
    return controlleurMessagePrive((const client **)listeClients, (const client *)clientCourant,
                                   messagePrive);
  }

  if (typeMessage == SALON) {
    Salon salon = *(Salon *)message;
    return controlleurSalon(clientCourant, listeSalons, salon);
  }

  return -1;
}

static int controlleurMessageBroadcast(const client **listeClients, const client *clientCourant,
                                       MessageBroadcast messageBroadcast) {
  strcpy(messageBroadcast.expediteur, clientCourant->nom);
  strcpy(messageBroadcast.nomSalon, clientCourant->nomSalon);
  broadcast(listeClients, clientCourant, &messageBroadcast);
  printf("%s broadcast : \"%s\"\n", clientCourant->nom, messageBroadcast.message);
  return 0;
}

static int controlleurAttributionPseudo(const client **listeClients, client *clientCourant,
                                        AttributionPseudo pseudo) {
  if (pseudoExiste(listeClients, pseudo.pseudo)) {
    envoyerMessageAlerte(clientCourant, "Ce pseudonyme est déjà utilisé.", AVERTISSEMENT);
    return -1;
  }

  strcpy(clientCourant->nom, pseudo.pseudo);
  return 0;
}

static int controlleurMessagePrive(const client **listeClients, const client *clientCourant,
                                   MessagePrive messagePrive) {
  printf("Destinataire : %s\n", messagePrive.destinataire);
  if (strcmp(messagePrive.destinataire, clientCourant->nom) == 0) {
    envoyerMessageAlerte(clientCourant, "Vous ne pouvez pas vous envoyer un message privé.",
                         AVERTISSEMENT);
    return -1;
  }

  if (strlen(messagePrive.message) == 0) {
    envoyerMessageAlerte(clientCourant, "Vous ne pouvez pas envoyer un message vide.",
                         AVERTISSEMENT);
    return -1;
  }

  strcpy(messagePrive.expediteur, clientCourant->nom);
  for (int i = 0; i < NB_CLIENTS_MAX; i++) {
    if (listeClients[i] != NULL && strcmp(listeClients[i]->nom, messagePrive.destinataire) == 0) {
      if (send(listeClients[i]->socket, &messagePrive, TAILLE_MESSAGE_TCP, 0) < 0)
        return -1;
      return 0;
    }
  }

  envoyerMessageAlerte(clientCourant, "Ce client n'existe pas.", AVERTISSEMENT);
  return -1;
}

static int controlleurSalon(client *clientCourant, char ** listeSalons, Salon salon) {
  const int idSalon = salon.idSalon;
  

  if (strcmp(listeSalons[idSalon], clientCourant->nomSalon) == 0) {
    envoyerMessageAlerte(clientCourant, "Vous êtes déjà dans ce salon.", AVERTISSEMENT);
    return -1;
  }

  if (strlen(listeSalons[idSalon]) == 0) {
    envoyerMessageAlerte(clientCourant, "Vous ne pouvez pas rejoindre un salon vide.",
                         AVERTISSEMENT);
    return -1;
  }

  strcpy(clientCourant->nomSalon, listeSalons[idSalon]);
  return 0;
}

/**
 * @brief Envoie un message à tous les clients connectés, sauf le client courant.
 * @param messageFormate Pointeur vers le message à envoyer (peut être de n'importe quel type).
 * @return int Retourne 0 si le message est envoyé avec succès, -1 en cas d'erreur
 * d'envoi.
 */
static int broadcast(const client **listeClients, const client *clientCourant,
                     const void *message) {
  for (int i = 0; i < NB_CLIENTS_MAX; i++) {
    if (listeClients[i] != NULL && listeClients[i] != clientCourant) {
      if (send(listeClients[i]->socket, message, TAILLE_MESSAGE_TCP, 0) < 0)
        return -1;
    }
  }

  return 0;
}

int envoyerMessageAlerte(const client *clientCourant, char *message, TypeAlerte typeAlerte) {
  MessageAlerte messageAlerte;
  messageAlerte.typeMessage = MESSAGE_ALERTE;
  messageAlerte.typeAlerte = typeAlerte;
  strcpy(messageAlerte.message, message);
  if (send(clientCourant->socket, &messageAlerte, TAILLE_MESSAGE_TCP, 0) < 0)
    return -1;
  return 0;
}
