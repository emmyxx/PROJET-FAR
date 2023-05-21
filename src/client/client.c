#include "../include/client/client.h"
#include "../include/client/formateur.h"
#include "../include/common.h"
#include "../include/constantes.h"
#include "../include/typesMessage.h"
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  gestionnaireArguments(argc, argv);

  // Arrête le programme proprement lorsqu'on appuie sur Ctrl+C
  signal(SIGINT, gestionnaireSignal);

  // Se connecte au serveur
  const char *ipServeur = argv[1];
  const int portServeur = atoi(argv[2]);
  char *pseudo = argv[3];
  int socketServeur = creerConnexionServeur(ipServeur, portServeur);

  if (strlen(pseudo) > TAILLE_PSEUDO) {
    afficherMessageAlerte("Erreur : le pseudo est trop long.\n", ERREUR);
    exit(EXIT_FAILURE);
  }

  // Envoie le pseudo passé en argument au serveur
  AttributionPseudo attributionPseudo;
  attributionPseudo.typeMessage = PSEUDO;
  strcpy(attributionPseudo.pseudo, pseudo);
  if (send(socketServeur, &attributionPseudo, TAILLE_MESSAGE_TCP, 0) == -1)
    gestionnaireErreur("Erreur lors de l'envoi du pseudo au serveur");

  pthread_t threadRecevoir, threadEnvoyer;
  if (pthread_create(&threadEnvoyer, NULL, threadEnvoiMessages, &socketServeur) != 0)
    gestionnaireErreur("Erreur lors de la création du thread d'envoi");

  if (pthread_create(&threadRecevoir, NULL, threadReceptionMessages, &socketServeur) != 0)
    gestionnaireErreur("Erreur lors de la création du thread de réception");

  pthread_join(threadEnvoyer, NULL);
  pthread_join(threadRecevoir, NULL);

  close(socketServeur);
  return 0;
}

void gestionnaireArguments(int argc, char *argv[]) {
  if (argc != NB_ARGS_ATTENDUS) {
    fprintf(stderr, "Erreur : %d arguments attendus, mais %d ont été fournis.\n", NB_ARGS_ATTENDUS,
            argc);
    fprintf(stderr, "Utilisation : %s <adresse_ip> <port> <pseudo>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

int creerConnexionServeur(const char *ipServeur, const int portServeur) {
  int socketServeur = socket(PF_INET, SOCK_STREAM, 0);

  if (socketServeur == -1)
    gestionnaireErreur("Erreur de création de la socket");

  // Configuration de l'adresse du serveur
  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  inet_pton(AF_INET, ipServeur, &(adresseEcouteur.sin_addr));
  adresseEcouteur.sin_port = htons(portServeur);
  socklen_t longueurAdresse = sizeof(struct sockaddr_in);

  // Connexion à la socket serveur
  if (connect(socketServeur, (struct sockaddr *)&adresseEcouteur, longueurAdresse) == -1)
    gestionnaireErreur("Erreur de connexion");

  return socketServeur;
}

void *threadEnvoiMessages(void *arg) {
  const int socketServeur = *(int *)arg;
  char saisie[TAILLE_SAISIE_CLIENT];

  while (true) {
    while (entrerMessage(saisie, TAILLE_SAISIE_CLIENT) >= 0 &&
           routageEnvoiMessage((const char *)saisie, socketServeur) >= 0)
      ;
  }

  // TODO Fermer le thread
  exit(EXIT_FAILURE);
}

void *threadReceptionMessages(void *arg) {
  int socketServeur = *(int *)arg;
  char messageRecu[TAILLE_MESSAGE_TCP];
  int reponse = 1;

  while (true) {
    reponse = recv(socketServeur, messageRecu, TAILLE_MESSAGE_TCP, 0);

    if (reponse < 0)
      gestionnaireErreur("Erreur lors de la reception du message");

    if (reponse == 0) {
      puts("⚠️  \033[31mUne erreur serveur est survenue, vous avez été "
           "déconnecté.\033[0m");
      exit(EXIT_FAILURE);
    }

    routageMessageRecu(messageRecu);
  }

  exit(EXIT_FAILURE);
}

int routageMessageRecu(void *messageRecu) {
  const TypeMessage typeMessage = *(TypeMessage *)messageRecu;

  if (typeMessage == MESSAGE_BROADCAST) {
    const MessageBroadcast messageBroadcast = *(MessageBroadcast *)messageRecu;
    recevoirMessageBroadcast(messageBroadcast);
    return 0;
  }

  if (typeMessage == MESSAGE_ALERTE) {
    MessageAlerte messageAlerte = *(MessageAlerte *)messageRecu;
    afficherMessageAlerte(messageAlerte.message, messageAlerte.typeAlerte);
    return 0;
  }

  if (typeMessage == MESSAGE_PRIVE) {
    MessagePrive messagePrive = *(MessagePrive *)messageRecu;
    recevoirMessagePrive(messagePrive);
    return 0;
  }

  return -1;
}

int recevoirMessageBroadcast(const MessageBroadcast messageBroadcast) {
  printf("%s : %s\n", messageBroadcast.expediteur, messageBroadcast.message);
  return 0;
}

int afficherMessageAlerte(char *message, TypeAlerte typeAlerte) {
  if (typeAlerte == INFORMATION) {
    printf("\033[3;37m%s\033[0m\n",
           message); // texte en bleu/gris clair et en italique
    return 0;
  } else if (typeAlerte == AVERTISSEMENT) {
    printf("🔔  \033[33m%s\033[0m\n", message); // texte en orange
    return 0;
  } else if (typeAlerte == ERREUR) {
    printf("⚠️  \033[1;31m%s\033[0m\n",
           message); // texte en gras et en rouge
    return 0;
  } else {
    return -1;
  }
}

int recevoirMessagePrive(const MessagePrive messagePrive) {
  printf("\033[0;34m%s : %s\033[0m\n", messagePrive.expediteur, messagePrive.message);
  return 0;
}

int entrerMessage(char *message, const int tailleMessage) {
  if (fgets(message, tailleMessage, stdin) == NULL || message[0] == '\n') {
    return -1;
  }

  if ((int)strlen(message) == tailleMessage - 1 && message[tailleMessage - 2] != '\n') {
    printf("⚠️  \033[31mLe message est trop long\033[0m\n");
    nettoyerBufferEntree();
    return -1;
  }

  // Remplacement du caractère de nouvelle ligne par un caractère nul
  strtok(message, "\n");
  return 0;
}

// Si l'utilisateur entre plus de caractères que la taille du message, les
// caractères supplémentaires sont stockés dans le buffer d'entrée, ce qui peut
// poser problème pour les prochains appels à fgets
void nettoyerBufferEntree() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void gestionnaireSignal(int signum) {
  if (signum == SIGINT) {
    arreterCommunication();
  }
}

void arreterCommunication() {
  printf("\nDeconnexion de la messagerie.\n");
  exit(EXIT_SUCCESS);
}

int afficherManuel() {
  FILE *fichier;
  char ch;

  // Ouvrir le fichier en mode lecture (read)
  fichier = fopen("manuel.txt", "r");

  // Vérifier si l'ouverture a réussi
  if (fichier == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return -1;
  }

  // Lire et afficher le contenu du fichier caractère par caractère
  while ((ch = fgetc(fichier)) != EOF) {
    putchar(ch);
  }

  // Fermer le fichier
  fclose(fichier);
  return 0;
}

int routageEnvoiMessage(const char *saisie, const int socketServeur) {
  // Fait une copie de saisie pour ne pas modifier l'originale
  char copieSaisie[TAILLE_SAISIE_CLIENT];
  strcpy(copieSaisie, saisie);

  // Exemple : "/mp pseudo message" -> "/mp"
  const char *nomCommande = strtok(copieSaisie, " ");

  // Vérifie que le nom de la commande n'est pas trop long
  if (strlen(nomCommande) >= TAILLE_SAISIE_CLIENT - 1) {
    printf("⚠️  \033[31mLa commande est trop longue\033[0m\n");
    return -1;
  }

  // ==========================================================================
  //            Commandes qui n'envoient pas de message au serveur
  // ==========================================================================

  if (strcmp(nomCommande, "/fin") == 0) {
    printf("Deconnexion de la messagerie.\n");
    exit(EXIT_SUCCESS); // TODO ne pas faire un exit
  }

  else if (strcmp(nomCommande, "/man") == 0) {
    afficherManuel();
    return 0;
  }

  // Afficher les fichiers locaux
  else if (strcmp(nomCommande, "/afl") == 0) {
    if (afficherFichiers(CHEMIN_DOSSIER_FICHIERS_LOCAUX) == -1)
      return -1;

    return 0;
  }

  // ==========================================================================
  //              Commandes qui envoient un message au serveur
  // ==========================================================================

  char *messageFormate = formater(saisie);

  if (messageFormate == NULL) {
    afficherMessageAlerte("Paramètres de la commande invalides.", AVERTISSEMENT);
    return -1;
  }

  if (*(TypeMessage *)messageFormate == INFORMATIONS_FICHIER) {
    argsThreadFichier args = {socketServeur, *(InformationsFichier *)messageFormate};
    pthread_t idthreadEnvoiFichier;

    if (pthread_create(&idthreadEnvoiFichier, NULL, threadEnvoiFichier, &args) != 0)
      gestionnaireErreur("Erreur lors de la création du thread d'envoi de fichier");

    // Libère les ressources du thread dès qu'il a fini son exécution
    pthread_detach(idthreadEnvoiFichier);
  }

  else if (send(socketServeur, messageFormate, TAILLE_MESSAGE_TCP, 0) < 0) {
    printf("⚠️  \033[31mErreur lors de l'envoi du message: %s\033[0m\n", strerror(errno));
    free(messageFormate);
    return -2;
  }

  free(messageFormate);
  return 0;
}

int afficherFichiers(const char *cheminDossier) {
  size_t nombreFichiers = 0;
  struct dirent *informationsFichiers = recupererTableauFichiers(&nombreFichiers, cheminDossier);

  if (informationsFichiers == NULL) {
    free(informationsFichiers);
    return -1;
  }

  if (nombreFichiers == 0) {
    afficherMessageAlerte("Vous n'avez aucun fichier dans votre dossier.", AVERTISSEMENT);
    free(informationsFichiers);
    return -1;
  }

  for (size_t i = 0; i < nombreFichiers; i++) {
    printf("[%ld] %s\n", i + 1, informationsFichiers[i].d_name);
  }

  free(informationsFichiers);
  return 0;
}

struct dirent *recupererTableauFichiers(size_t *taille, const char *cheminDossier) {
  *taille = 0;
  DIR *dossier;
  struct dirent *pointeurEntitesDossier;
  // Tableau qui ne comprend que les entités qui sont des fichiers (et non pas des dossiers)
  struct dirent *informationsFichiers;
  // Correspond à la taille qu'ajoutera chaque realloc.
  // Permet d'évite de faire trop souvent des realloc, qui est une opération coûteuse.
  size_t tailleBlockMemoire = 10;
  // La taille allouée dans la mémoire (⚠️ différente de la taille du tableau de dirent !)
  size_t tailleAllouee = tailleBlockMemoire;

  if (strlen(cheminDossier) == 0 || strlen(cheminDossier) > NAME_MAX) {
    errno = EINVAL;
    fprintf(stderr, "La taille du nom du dossier est trop courte ou trop longue.");
    return NULL;
  }

  dossier = opendir(cheminDossier);
  if (dossier == NULL) {
    perror("Erreur lors de l\'ouverture du dossier des fichiers locaux");
    return NULL;
  }

  // Créer un espace mémoire assez grand pour éviter de faire trop de realloc
  informationsFichiers = (struct dirent *)malloc(sizeof(struct dirent) * 100);
  if (informationsFichiers == NULL) {
    perror("Erreur lors de l\'allocation de la mémoire");
    closedir(dossier);
    return NULL;
  }

  while ((pointeurEntitesDossier = readdir(dossier)) != NULL) {
    // Sélectionne les fichiers, pas les dossiers
    // Je n'ai pas trouvé sur internet de doc qui parlait de d_type, c'est Chat GPT qui m'a donné ça
    if (pointeurEntitesDossier->d_type != DT_REG)
      continue;

    // Réalloue de la mémoire si nécessaire
    if (*taille > tailleAllouee) {
      tailleAllouee = tailleAllouee + tailleBlockMemoire;
      informationsFichiers = realloc(informationsFichiers, tailleAllouee);
      if (informationsFichiers == NULL) {
        perror("Echec de la réallocation de la mémoire");
        free(informationsFichiers);
        closedir(dossier);
        return NULL;
      }
    }

    informationsFichiers[*taille] = *pointeurEntitesDossier;
    (*taille)++;
  }

  closedir(dossier);
  return informationsFichiers;
}

/* -------------------------------------------------------------------------- */
/*                       Réception et envoi des fichiers */
/* --------------------------------------------------------------------------
 */
void *threadEnvoiFichier(void *arg) {
  const argsThreadFichier argsThread = *(argsThreadFichier *)arg;
  const int socketServeur = argsThread.socketServeur;
  const InformationsFichier informationsFichier = argsThread.informationsFichier;
  char cheminFichier[PATH_MAX];
  // struct stat statsFichier;

  // Récupération du chemin du fichier
  strcpy(cheminFichier, CHEMIN_DOSSIER_FICHIERS_LOCAUX);
  strcat(cheminFichier, informationsFichier.nomFichier);

  if (send(socketServeur, &informationsFichier, TAILLE_MESSAGE_TCP, 0) < 0) {
    printf("⚠️  \033[31mErreur lors de l'envoi du message: %s\033[0m\n", strerror(errno));
    pthread_exit(NULL);
  }

  // if (stat(cheminFichier, &statsFichier) == -1) {
  //   perror("Echec lors de la récupération des informations du fichier)");
  //   errno = EINVAL;
  //   return NULL;
  // }

  pthread_exit(NULL);
}

void *threadReceptionFichier(void *arg);
