#include <dirent.h>
#include <sys/stat.h>

#include "../include/client/client.h"
#include "../include/client/formateur.h"
#include "../include/common.h"
#include "../include/constantes.h"
#include "../include/typesMessage.h"

static MessageBroadcast *formaterEnBroadcast(const char *saisieClient);
static AttributionPseudo *formaterEnAttributionPseudo(char *saisieClient);
static MessagePrive *formaterEnMessagePrive(char *saisieClient);
static Fichier *formaterEnFichier(char *saisieClient);
static Salon *formaterEnSalon(char *saisieClient);
int recupererTailleFichier(const char *nomFichier);

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

  if (strcmp(nomCommande, "/envoyer") == 0)
    return formaterEnFichier(saisieSansCommande);

  if (strcmp(nomCommande, "/salon") == 0)
    return formaterEnSalon(saisieSansCommande);

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

static Fichier *formaterEnFichier(char *saisieClient) {
  size_t nombreFichiers = 0;
  struct dirent *tableauFichiers;

  const char *numeroFichierChaine = strtok(saisieClient, " ");
  if (numeroFichierChaine == NULL) {
    perror("Erreur strtok");
    return NULL;
  }

  const int numeroFichierInt = atoi(numeroFichierChaine) - 1;

  tableauFichiers = recupererTableauFichiers(&nombreFichiers, CHEMIN_DOSSIER_FICHIERS_LOCAUX);

  if (tableauFichiers == NULL || nombreFichiers == 0 || numeroFichierInt < 0 ||
      numeroFichierInt >= (int)nombreFichiers) {
    errno = EINVAL;
    free(tableauFichiers);
    return NULL;
  }

  // Récupération du fichier corresondant au numéro donné par le client
  const struct dirent fichierSelectionne = tableauFichiers[numeroFichierInt];

  Fichier *fichier = (Fichier *)malloc(sizeof(Fichier));
  fichier->typeMessage = FICHIER;
  strcpy(fichier->nomFichier, fichierSelectionne.d_name);
  const int tailleFichier = recupererTailleFichier(fichierSelectionne.d_name);

  if (tailleFichier == -1) {
    perror("Erreur récupération taille fichier");
    free(tableauFichiers);
    return NULL;
  }

  fichier->tailleFichier = tailleFichier;

  free(tableauFichiers);
  return fichier;
}

static Salon *formaterEnSalon(char *saisieClient) {
  const char *idSalon = strtok(saisieClient, " ");
  if (idSalon == NULL) {
    errno = EINVAL;
    return NULL;
  }

  const int idSalonInt = atoi(idSalon);

  if (idSalonInt > NB_SALONS_MAX - 1 || idSalonInt < 0) {
    errno = EINVAL;
    return NULL;
  }

  Salon *salon = (Salon *)malloc(sizeof(Salon));
  salon->typeMessage = SALON;
  salon->idSalon = idSalonInt;

  return salon;
}

int recupererTailleFichier(const char *nomFichier) {
  char cheminFichier[PATH_MAX];
  strcpy(cheminFichier, CHEMIN_DOSSIER_FICHIERS_LOCAUX);
  strcat(cheminFichier, nomFichier);

  struct stat st;
  if (stat(cheminFichier, &st) != 0) {
    perror("Erreur stat");
    return -1;
  }
  return st.st_size;
}
