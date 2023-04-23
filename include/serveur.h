int accepterClient(int socketEcouteur);
int creerSocketEcouteur(int port, int nbClientsEnAttente);

typedef struct client
{
    int socket;
    char *nom;
    int estConnecte;
} client;

int demarrerConversation(client *emetteur, client *recepteur, const int tailleMessage);