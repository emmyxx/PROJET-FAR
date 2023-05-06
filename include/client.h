#define NB_ARGS_ATTENDUS 3

int creerConnexionServeur(const char *ipServeur, const int portServeur);
void fermerSocketServeur(int socketServeur);
void *entrerEtEnvoyerMessages(void *arg);
void *recevoirEtAfficherMessages(void *arg);
int entrerMessage(char *message, const int tailleMessage);
void nettoyerBufferEntree();