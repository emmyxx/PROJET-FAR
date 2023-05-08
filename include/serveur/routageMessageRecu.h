#ifndef ROUTAGE_MESSAGE_RECU_H
#define ROUTAGE_MESSAGE_RECU_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "gestionnaireClients.h"

/**
 * @brief Appelle le controlleur correspondant au type de message reçu.
 * @param listeClients Pointeur vers un tableau de pointeurs de structures client.
 * @param clientCourant Pointeur vers la structure client qui a envoyé le message.
 * @param message Pointeur vers le message reçu.
 * @return int Retourne 0 si le message est traité avec succès, -1 en cas d'erreur.
 */
int routageMessageRecu(client **listeClients, client *clientCourant, void *message);

int envoyerMessageAlerte(const client *clientCourant, char *message, TypeAlerte typeAlerte);

#endif
