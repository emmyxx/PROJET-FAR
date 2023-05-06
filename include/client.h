#define NB_ARGS_ATTENDUS 3

int creerConnexionServeur(const char *ipServeur, const int portServeur);
void fermerSocketServeur(int socketServeur);
void *entrerEtEnvoyerMessages(void *arg);
void *recevoirEtAfficherMessages(void *arg);

/**
 * @brief Lit une chaîne de caractères à partir de l'entrée standard.
 * @param message La chaîne de caractères lue depuis l'entrée standard.
 * @param tailleMessage La taille du tableau.
 * @return 0 si la lecture de la chaîne est réussie, -1 si aucun caractère
 * n'est entré ou si l'utilisateur a entré plus de tailleMessage - 1 caractères.
 */
int entrerMessage(char *message, const int tailleMessage);
void nettoyerBufferEntree();

