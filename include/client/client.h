#define NB_ARGS_ATTENDUS 4


/* -------------------------------------------------------------------------- */
/*                           Divers                                           */
/* -------------------------------------------------------------------------- */
int creerConnexionServeur(const char *ipServeur, const int portServeur);
void gestionnaireSignal(int signum);
void arreterCommunication();
int afficherManuel();


/* -------------------------------------------------------------------------- */
/*           Affichage et sélection d'un fichier dans un répertoire           */
/* -------------------------------------------------------------------------- */

// Structure qui permet d'indiquer la taille d'un tableau de structures InformationsFichier.
typedef struct {
    InformationsFichier* tableau;
    int taille;
} TableauInformationsFichiers;

/** 
 * @brief Récupère les informations des fichiers présents dans le dossier
 * @param cheminDossier Le chemin du dossier où l'on veut récupérer les informations des fichiers.
 * @return Une structure contenant un tableau d'informations de fichiers 
 * ainsi que sa taille ou NULL si une erreur s'est produite.
 * @note Il faut appeler la fonction libererTableauInformationsFichiers pour libérer la mémoire allouée.
*/
TableauInformationsFichiers recupererFichier(char *cheminDossier);

/** 
 * @brief Affiche les informations d'une structure TableauInformationsFichiers.
 * @param tableau La structure à afficher.
*/
void afficherFichiers(TableauInformationsFichiers tableau);

/**
 * @brief demande à l'utilisateur de choisir un fichier en entrant son index 
 * dans le tableau de la structure TableauInformationsFichiers.
 * @param tableau La structure contenant le tableau d'informations de fichiers.
 * @return L'index du fichier choisi par l'utilisateur, ou -1 si le tableau est vide.
*/
 InformationsFichier choisirFichier(TableauInformationsFichiers tableau);

/** 
 * @brief Libère la mémoire allouée pour le tableau dans la structure.
 * @param tableau La structure dont la mémoire du tableau doit être libérée.
*/
void libererTableauInformationsFichiers(TableauInformationsFichiers* tableau);


/* -------------------------------------------------------------------------- */
/*                    Réception et traitement des messages                    */
/* -------------------------------------------------------------------------- */
void *threadReceptionMessages(void *arg);
int routageMessageRecu(void *messageRecu);
int recevoirMessageBroadcast(const MessageBroadcast messageBroadcast);
int afficherMessageAlerte(char *message, TypeAlerte typeAlerte);
int recevoirMessagePrive(const MessagePrive messagePrive);


/* -------------------------------------------------------------------------- */
/*                  Saisie, formattage et envoi des messages                  */
/* -------------------------------------------------------------------------- */
void *threadEnvoiMessages(void *arg);

/**
 * @brief Lit une chaîne de caractères à partir de l'entrée standard.
 * @param message La chaîne de caractères lue depuis l'entrée standard.
 * @param tailleMessage La taille du tableau.
 * @return 0 si la lecture de la chaîne est réussie, -1 si aucun caractère
 * n'est entré ou si l'utilisateur a entré plus de tailleMessage - 1 caractères.
 */
int entrerMessage(char *message, const int tailleMessage);

int routageEnvoiMessage(const char *saisie, const int socketServeur);
void nettoyerBufferEntree();

/* -------------------------------------------------------------------------- */
/*                       Réception et envoi des fichiers                      */
/* -------------------------------------------------------------------------- */
void *threadEnvoiFichier(void *arg);
void *threadReceptionFichier(void *arg);