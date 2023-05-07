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

static MessagePrive *formaterEnMessagePrive(char *saisieClient)
{
    const char *pseudoDestinataire = strtok(saisieClient, " ");
    if (pseudoDestinataire == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    MessagePrive *messagePrive = (MessagePrive *)malloc(sizeof(MessagePrive));
    messagePrive->typeMessage = MESSAGE_PRIVE;
    strcpy(messagePrive->destinataire, pseudoDestinataire);
    char *message = strtok(NULL, "");
    if (message == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    strcpy(messagePrive->message, message);

    return messagePrive;
}

int afficherManuel()
{
    FILE *fichier;
    char ch;

    // Ouvrir le fichier en mode lecture (read)
    fichier = fopen("manuel.txt", "r");

    // Vérifier si l'ouverture a réussi
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return -1;
    }

    // Lire et afficher le contenu du fichier caractère par caractère
    while ((ch = fgetc(fichier)) != EOF)
    {
        putchar(ch);
    }

    // Fermer le fichier
    fclose(fichier);
    return 0;
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

    if (strcmp(nomCommande, "/fin") == 0)
    {
        printf("\nDeconnexion de la messagerie.\n");
        exit(EXIT_SUCCESS);
    }

    if (strcmp(nomCommande, "/mp") == 0)
        return formaterEnMessagePrive(saisieSansCommande);

    if (strcmp(nomCommande, "/man") == 0)
    {
        afficherManuel();
        return NULL;
    }

    errno = EINVAL;
    return NULL;
}
