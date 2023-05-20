#ifndef GESTIONNAIRE_CLIENTS_H
#define GESTIONNAIRE_CLIENTS_H

#include "../include/constantes.h"
#include <stdbool.h>

typedef struct client {
  int socket;
  char nom[TAILLE_PSEUDO];
  bool estConnecte;
} client;

/**
 * @brief Crée un tableau de clients.
 * @return Un pointeur vers le tableau de clients créé.
 * @note L'appelant doit libérer la mémoire allouée par cette fonction.
 */
client **creerTableauClients();

/**
 * @brief Détruit un tableau de clients.
 * @param listeClients Pointeur vers un tableau de pointeurs de structures client.
 */
void detruireTableauClients(client **listeClients);

/**
 * @brief Calcule le nombre de clients connectés dans le tableau des clients.
 * @param listeClients Pointeur vers un tableau de pointeurs de structures client.
 * @return int Retourne le nombre de clients connectés, entre 0 et NB_CLIENTS_MAX.
 */
int avoirNombreClientsConnectes(const client **listeClients);

/**
 * @brief Ajoute un client au tableau des clients.
 * @param listeClients Pointeur vers un tableau de pointeurs de structures client.
 * @param client Pointeur vers la structure client à ajouter au tableau.
 * @return int Retourne 0 si le client est ajouté avec succès, -1 si le tableau est plein.
 */
int ajouterClient(client **listeClients, client *client);

/**
 * @brief Supprime un client du tableau des clients.
 * @param listeClients Pointeur vers un tableau de pointeurs de structures client.
 * @param client Pointeur vers la structure client à supprimer du tableau.
 * @return int Retourne 0 si le client est supprimé avec succès, -1 si le client
 * n'est pas présent dans le tableau.
 */
int supprimerClient(client **listeClients, client *client);

/**
 * @brief Vérifie si un pseudo est déjà utilisé par un client.
 * @param listeClients Pointeur vers un tableau de pointeurs de structures client.
 * @param pseudo Pointeur vers une chaîne de caractères contenant le pseudo à vérifier.
 * @return bool Retourne true si le pseudo est déjà utilisé, false sinon.
 */
bool pseudoExiste(const client **listeClients, const char *pseudo);

#endif
