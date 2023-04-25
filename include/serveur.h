#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define NB_ARGS_ATTENDUS 2
#define NB_CLIENTS_EN_ATTENTE 8
#define NB_CLIENTS_MAX 3

typedef struct client
{
    int socket;
    char *nom;
} client;

int demarrerConversation(client *emetteur, client *recepteur, const int tailleMessage);

typedef struct argsThread
{
    client *client;
    client **clients;
} argsThread;

int creerSocketEcouteur(int port, int nbClientsEnAttente);

void *broadcast(void *arg);

/**
 * @brief Accepte une connexion client entrante sur la socket d'écoute.
 * @param socketEcouteur Le descripteur de la socket d'écoute.
 * @return int Le descripteur de la socket client si la connexion est acceptée avec succès, -1 en cas d'échec.
 */
int accepterClient(const int socketEcouteur);

/**
 * @brief Génère un nom de client et l'alloue dans la mémoire. Le but est de différencier les clients.
 *
 * Cette fonction génère un nom de client sous la forme "clientX", où X est un nombre aléatoire
 * entre 1 et 1000. Le nom généré est ensuite alloué en mémoire.
 *
 * @param nomClient Pointeur vers le nom du client généré.
 * @return int Retourne -1 en cas d'échec de l'allocation de mémoire, 0 sinon.
 */
int genererNomClient(char **nomClient);

/**
 * @brief Calcule le nombre de clients connectés dans le tableau des clients.
 * @param clients Pointeur vers un tableau de pointeurs de structures client.
 * @return int Retourne le nombre de clients connectés,
 * entre 0 et NB_CLIENTS_MAX.
 */
int avoirNombreClientsConnectes(client **clients);

/**
 * @brief Ajoute un client au tableau des clients.
 * @param clients Pointeur vers un tableau de pointeurs de structures client.
 * @param clientAAjouter Pointeur vers la structure client à ajouter au tableau.
 * @return int Retourne 0 si le client est ajouté avec succès, -1 si le tableau est plein.
 */
int ajouterAuTableau(client **clients, client *clientAAjouter);

/**
 * @brief Supprime un client du tableau des clients.
 * @param clients Pointeur vers un tableau de pointeurs de structures client.
 * @param clientASupprimer Pointeur vers la structure client à supprimer du tableau.
 * @return int Retourne 0 si le client est supprimé avec succès, -1 si le client n'est pas présent dans le tableau.
 */
int supprimerDuTableau(client **clients, client *clientASupprimer);