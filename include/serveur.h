#define NB_ARGS_ATTENDUS 2
#define NB_CLIENTS_EN_ATTENTE 8
#define NB_CLIENTS_MAX 3


/* -------------------------------------------------------------------------- */
/*                          Mise en place du serveur                          */
/* -------------------------------------------------------------------------- */
int creerSocketEcouteur(int port, int nbClientsEnAttente);


/* -------------------------------------------------------------------------- */
/*                             Gestion des clients                            */
/* -------------------------------------------------------------------------- */
typedef struct client
{
    int socket;
    char nom[TAILLE_PSEUDO];
    bool estConnecte;
} client;

typedef struct argsThread
{
    client *client;
    client **clients;
} argsThread;

/**
 * @brief Accepte une connexion client entrante sur la socket d'écoute.
 * @param socketEcouteur Le descripteur de la socket d'écoute.
 * @return int Le descripteur de la socket client si la connexion est acceptée avec succès, -1 en cas d'échec.
 */
int accepterClient(const int socketEcouteur);

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
bool pseudoExiste(const client **listeClients, const char *pseudo);
int broadcast(const client **listeClients, const client *clientCourant, const void *messageFormate);
