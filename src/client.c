#include "../include/common.h"
#include "../include/client.h"

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);

  const char *ipServeur = argv[1];
  const int portServeur = atoi(argv[2]);
  int socketServeur = creerConnexionServeur(ipServeur, portServeur);

  pthread_t threadRecevoir, threadEnvoyer;
  if (pthread_create(&threadEnvoyer, NULL, entrerEtEnvoyerMessages, &socketServeur) != 0)
    gestionnaireErreur("Erreur lors de la création du thread d'envoi");

  if (pthread_create(&threadRecevoir, NULL, recevoirEtAfficherMessages, &socketServeur) != 0)
    gestionnaireErreur("Erreur lors de la création du thread de réception");

  pthread_join(threadEnvoyer, NULL);
  pthread_join(threadRecevoir, NULL);

  close(socketServeur);
  return 0;
}

void gestionnaireArguments(int argc, char *argv[])
{
  if (argc != NB_ARGS_ATTENDUS)
  {
    fprintf(stderr,
            "Erreur : %d arguments attendus, mais %d ont été fournis.\n",
            NB_ARGS_ATTENDUS, argc);
    fprintf(stderr, "Utilisation : %s <adresse_ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

int creerConnexionServeur(const char *ipServeur, const int portServeur)
{
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

void *entrerEtEnvoyerMessages(void *arg)
{
  int socketServeur = *(int *)arg;
  char saisieClient[TAILLE_SAISIE_CLIENT];

  while (strcmp(saisieClient, "fin") != 0)
  {
    char *messageFormate = NULL;

    while (messageFormate == NULL)
    {
      while (entrerMessage(saisieClient, TAILLE_SAISIE_CLIENT) != 0)
        ;
      messageFormate = formaterSaisieClient(saisieClient);
    }

    if (send(socketServeur, messageFormate, TAILLE_MESSAGE_TCP, 0) < 0)
      gestionnaireErreur("Erreur lors de l'envoi du message");

    free(messageFormate);
  }

  puts("Vous avez quitté la conversation");
  exit(EXIT_SUCCESS);
}

void *recevoirEtAfficherMessages(void *arg)
{
  int socketServeur = *(int *)arg;
  char messageRecu[TAILLE_MESSAGE_TCP];
  int reponse = 1;

  while (true)
  {
    reponse = recv(socketServeur, messageRecu, TAILLE_MESSAGE_TCP, 0);

    if (reponse < 0)
      gestionnaireErreur("Erreur lors de la reception du message");

    if (reponse == 0)
    {
      puts("⚠️  \033[31mUne erreur serveur est survenue, vous avez été déconnecté.\033[0m");
      exit(EXIT_FAILURE);
    }

    routageMessageRecu(messageRecu);
  }

  exit(EXIT_FAILURE);
}

int routageMessageRecu(void *messageRecu)
{
  const TypeMessage typeMessage = *(TypeMessage *)messageRecu;

  if (typeMessage == MESSAGE_BROADCAST)
  {
    const MessageBroadcast messageBroadcast = *(MessageBroadcast *)messageRecu;
    recevoirMessageBroadcast(messageBroadcast);
    return 0;
  }

  return -1;
}

int recevoirMessageBroadcast(const MessageBroadcast messageBroadcast) {
  printf("%s : %s\n", messageBroadcast.expediteur, messageBroadcast.message);

  return 0;
}

int entrerMessage(char *message, const int tailleMessage)
{
  if (fgets(message, tailleMessage, stdin) == NULL || message[0] == '\n')
  {
    // TODO standardiser messages d'erreurs en utilisant une fonction dédiée
    printf("⚠️  \033[31mLe message ne peut pas être vide\033[0m\n");
    return -1;
  }

  if ((int)strlen(message) == tailleMessage - 1 && message[tailleMessage - 2] != '\n')
  {
    printf("⚠️  \033[31mLe message est trop long\033[0m\n");
    nettoyerBufferEntree();
    return -1;
  }

  // Remplacement du caractère de nouvelle ligne par un caractère nul
  strtok(message, "\n");
  return 0;
}

// Si l'utilisateur entre plus de caractères que la taille du message, les caractères supplémentaires sont stockés dans le buffer d'entrée,
// ce qui peut poser problème pour les prochains appels à fgets
void nettoyerBufferEntree()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
