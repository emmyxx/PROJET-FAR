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

/**
 * @brief Formate la saisie d'un client de manière à ce qu'elle respecte la syntaxe du protocole.
 * @param messageTCP Le message TCP à envoyer au serveur.
 * @param saisieClient Le message à formater.
 * @return Un pointeur vers le message formaté, ou NULL si le message ne peut pas être formaté (avec 
 * le errno mis à EINVAL).
 */
char *formaterSaisieClient(char *messageTCP, const char *saisieClient);
void nettoyerBufferEntree();


// Fonctions de formatage des saisies des clients en un messages TCP respectant le protocole
// Ces fonctions supposent que le nom de la commande (ex: "/mp") est été retiré de la saisie,
// et que messageTCP est rempli de '\0'.

char *formaterEnBroadcast(char *messageTCP, char *saisieClient);
char *formaterEnPseudo(char *messageTCP, char *saisieClient);