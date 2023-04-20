int accepterClient(int socketEcouteur);
int creerSocketEcouteur(int port, int nbClientsEnAttente);
struct socketClients
{
    int clientEmetteur;
    int clientRecepteur;
};

int demarrerConversation(struct socketClients clients, int TAILLE_MESSAGE);
