/**
 * @brief Formate la saisie d'un client de manière à ce qu'elle respecte la syntaxe du protocole.
 * @param messageTCP Le message TCP à envoyer au serveur.
 * @param saisieClient Le message à formater.
 * @return Un pointeur vers le message formaté, ou NULL si le message ne peut pas être formaté (avec 
 * le errno mis à EINVAL).
 */
char *formaterSaisieClient(char *messageTCP, const char *saisieClient);
