int accepterClient(int socketEcouteur);
int creerSocketEcouteur(int port, int nbClients);

struct socketClients
{
    int clientEmetteur;
    int clientRecepteur;
};