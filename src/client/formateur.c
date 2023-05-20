#include "../include/client/formateur.h"
#include "../include/common.h"
#include "../include/constantes.h"
#include "../include/typesMessage.h"

static MessageBroadcast *formaterEnBroadcast(const char *saisieClient);
static AttributionPseudo *formaterEnAttributionPseudo(char *saisieClient);
static MessagePrive *formaterEnMessagePrive(char *saisieClient);
static InformationsFichier *formaterEnInformationsFichier(char *saisieClient);

void *formater(const char *saisie) {
  // Fait une copie de saisie pour ne pas modifier l'originale
  char copieSaisie[TAILLE_SAISIE_CLIENT];
  strcpy(copieSaisie, saisie);

  // Exemple : "/mp pseudo message" -> "/mp"
  const char *nomCommande = strtok(copieSaisie, " ");

  // Exemple : "/mp pseudo message" -> "pseudo message"
  char *saisieSansCommande = copieSaisie + strlen(nomCommande) + 1;

  if (saisie[0] != '/')
    return formaterEnBroadcast(saisie);

  if (strcmp(nomCommande, "/pseudo") == 0)
    return formaterEnAttributionPseudo(saisieSansCommande);

  if (strcmp(nomCommande, "/mp") == 0)
    return formaterEnMessagePrive(saisieSansCommande);

  if (strcmp(nomCommande, "/fichier") == 0)
    return formaterEnInformationsFichier(saisieSansCommande);

  printf("⚠️  \033[31mCommande inconnue\033[0m\n");

  errno = EINVAL;
  return NULL;
}

static MessageBroadcast *formaterEnBroadcast(const char *saisieClient) {
  MessageBroadcast *messageBroadcast = (MessageBroadcast *)malloc(sizeof(MessageBroadcast));
  messageBroadcast->typeMessage = MESSAGE_BROADCAST;
  strcpy(messageBroadcast->message, saisieClient);
  return messageBroadcast;
}

static AttributionPseudo *formaterEnAttributionPseudo(char *saisieClient) {
  const char *pseudo = strtok(saisieClient, " ");
  if (pseudo == NULL) {
    errno = EINVAL;
    return NULL;
  }

  AttributionPseudo *attributionPseudo = (AttributionPseudo *)malloc(sizeof(AttributionPseudo));
  attributionPseudo->typeMessage = PSEUDO;
  strcpy(attributionPseudo->pseudo, pseudo);

  return attributionPseudo;
}

static MessagePrive *formaterEnMessagePrive(char *saisieClient) {
  const char *pseudoDestinataire = strtok(saisieClient, " ");
  if (pseudoDestinataire == NULL) {
    errno = EINVAL;
    return NULL;
  }

  MessagePrive *messagePrive = (MessagePrive *)malloc(sizeof(MessagePrive));
  messagePrive->typeMessage = MESSAGE_PRIVE;
  strcpy(messagePrive->destinataire, pseudoDestinataire);
  char *message = strtok(NULL, "");
  if (message == NULL) {
    errno = EINVAL;
    return NULL;
  }
  strcpy(messagePrive->message, message);

  return messagePrive;
}

// FIXME code probablement à suppimer
static InformationsFichier *formaterEnInformationsFichier(char *saisieClient) {
  const char *nomFichier = strtok(saisieClient, " ");
  if (nomFichier == NULL) {
    errno = EINVAL;
    return NULL;
  }

  InformationsFichier *informationsFichier = (InformationsFichier *)malloc(sizeof(InformationsFichier));
  informationsFichier->typeMessage = INFORMATIONS_FICHIER;
  strcpy(informationsFichier->nomFichier, nomFichier);
  // TODO mettre dans la structure de données la taille du fichier

  return informationsFichier;
}
