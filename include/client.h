int creerConnexionServeur(const char *ipServeur, const int portServeur);
int entrerMessage(char *message);
void fermerSocketServeur(int socketServeur);
void nettoyerBufferEntree();
void gestionSignal(int sig);
int envoiMessage(int socket, char* msg);