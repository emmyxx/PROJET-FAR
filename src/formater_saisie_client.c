#include "../include/common.h"
#include "../include/formater_saisie_client.h"

static MessageBroadcast *formaterEnBroadcast(char *saisieClient)
{
    MessageBroadcast *messageBroadcast = (MessageBroadcast *)malloc(sizeof(MessageBroadcast));
    messageBroadcast->typeMessage = MESSAGE_BROADCAST;
    strcpy(messageBroadcast->message, saisieClient);
    return messageBroadcast;
}

static AttributionPseudo *formaterEnPseudo(char *saisieClient)
{
    const char *pseudo = strtok(saisieClient, " ");
    if (pseudo == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    AttributionPseudo *attributionPseudo = (AttributionPseudo *)malloc(sizeof(AttributionPseudo));
    attributionPseudo->typeMessage = PSEUDO;
    strcpy(attributionPseudo->pseudo, pseudo);

    return attributionPseudo;
}

void *formaterSaisieClient(char *saisie)
{
    // S'il n'y a pas de commande
    if (saisie[0] != '/')
        return formaterEnBroadcast(saisie);

    const char *nomCommande = strtok(saisie, " ");

    if (nomCommande == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    // Empêche le programme de planter lors de la création de saisieSansCommande
    // si l'utilisateur entre une commande trop longue
    if (strlen(nomCommande) >= TAILLE_SAISIE_CLIENT - 1)
    {
        errno = EINVAL;
        return NULL;
    }

    // "/mp pseudo message" -> "pseudo message"
    char *saisieSansCommande = saisie + strlen(nomCommande) + 1;

    if (strcmp(nomCommande, "/pseudo") == 0)
        return formaterEnPseudo(saisieSansCommande);

    errno = EINVAL;
    return NULL;
}