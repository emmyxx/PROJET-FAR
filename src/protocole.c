#include "../include/common.h"

static char *formaterEnBroadcast(char *messageTCP, char *saisieClient)
{
    strcat(messageTCP, "MESSAGE_BROADCAST ");
    strcat(messageTCP, saisieClient);
    return messageTCP;
}

static char *formaterEnPseudo(char *messageTCP, char *saisieClient)
{
    const char *pseudo = strtok(saisieClient, " ");
    if (pseudo == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    strcat(messageTCP, "PSEUDO ");
    strcat(messageTCP, pseudo);

    return messageTCP;
}

char *formaterSaisieClient(char *messageTCP, const char *saisie)
{
    // Remplit messageTCP de '\0' (au cas où il y aurait des caractères résiduels)
    memset(messageTCP, '\0', TAILLE_MESSAGE_TCP);

    char *copieSaisie = strdup(saisie);

    const char *nomCommande = strtok(copieSaisie, " ");

    if (nomCommande == NULL)
    {
        errno = EINVAL;
        free(copieSaisie);
        return NULL;
    }

    // Empêche le programme de planter lors de la création de saisieSansCommande
    // si l'utilisateur entre une commande trop longue
    if (strlen(nomCommande) >= TAILLE_SAISIE_CLIENT - 1)
    {
        errno = EINVAL;
        free(copieSaisie);
        return NULL;
    }

    // S'il n'y a pas de commande
    if (copieSaisie[0] != '/')
        messageTCP = formaterEnBroadcast(messageTCP, copieSaisie);

    // "/mp pseudo message" -> "pseudo message"
    char *saisieSansCommande = copieSaisie + strlen(nomCommande) + 1;

    if (strcmp(nomCommande, "/pseudo") == 0)
        messageTCP = formaterEnPseudo(messageTCP, saisieSansCommande);

    // Si la saisie du client ne respecte pas la syntaxe d'une commande
    if (messageTCP == NULL || messageTCP[0] == '\0')
    {
        errno = EINVAL;
        free(copieSaisie);
        return NULL;
    }

    // DEBUG
    printf("messageTCP : %s\n", messageTCP);

    return messageTCP;
}
