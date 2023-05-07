#define TAILLE_TYPE_MESSAGE 16

/**
 * @brief Formate la saisie d'un client de manière à ce qu'elle respecte la syntaxe du protocole.
 * @param messageTCP Le message TCP à envoyer au serveur.
 * @param saisieClient Le message à formater.
 * @return Un pointeur vers le message formaté, ou NULL si le message ne peut pas être formaté (avec 
 * le errno mis à EINVAL).
 */
void *formaterSaisieClient(char *saisie);

typedef enum {
    MESSAGE_BROADCAST,
    MESSAGE_PRIVE,
    PSEUDO,
} TypeMessage;

typedef struct
{
    TypeMessage typeMessage;
    char expediteur[TAILLE_PSEUDO ];
    char message[TAILLE_SAISIE_CLIENT];
} MessageBroadcast;

typedef struct
{
    TypeMessage typeMessage;
    char pseudo[TAILLE_PSEUDO];
} AttributionPseudo;

typedef struct
{
    char typeMessage[TAILLE_TYPE_MESSAGE];
    char expediteur[TAILLE_PSEUDO];
    char destinataire[TAILLE_PSEUDO];
    char message[TAILLE_SAISIE_CLIENT];
} MessagePrive;
