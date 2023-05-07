/**
 * @brief Formate la saisie d'un client de manière à ce qu'elle respecte la syntaxe du protocole.
 * @param saisie Le message à formater.
 * @return Un pointeur vers le message formaté, ou NULL si le message ne peut pas être formaté (avec 
 * le errno mis à EINVAL).
 * @warning Le message formaté est déclaré dans le tas.
 */
void *formaterSaisieClient(char *saisie);
