#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/common.h"
#include "../include/client.h"

#define NB_ARGS_ATTENDUS 3
#define TAILLE_MESSAGE 256

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);

  const char *ipServeur = argv[1];
  const int portServeur = atoi(argv[2]);
  const int socketServeur = creerConnexionServeur(ipServeur, portServeur);

  pthread_t threadRecevoir, threadEnvoyer;
  argsThread args = {socketServeur, TAILLE_MESSAGE};

  if (pthread_create(&threadEnvoyer, NULL, entrerEtEnvoyerMessages, &args) != 0)
    gestionnaireErreur("Erreur lors de la création du thread d'envoi");

  if (pthread_create(&threadRecevoir, NULL, recevoirEtAfficherMessages, &args) != 0)
    gestionnaireErreur("Erreur lors de la création du thread de réception");

  pthread_join(threadEnvoyer, NULL);
  pthread_join(threadRecevoir, NULL);

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
  // inet_pton convertit une chaîne de caractères représentant une adresse IP en une adresse réseau
  inet_pton(AF_INET, ipServeur, &(adresseEcouteur.sin_addr));
  adresseEcouteur.sin_port = htons(portServeur);
  socklen_t longueurAdresse = sizeof(struct sockaddr_in);

  // Connexion à la socket serveur
  if (connect(socketServeur, (struct sockaddr *)&adresseEcouteur, longueurAdresse) == -1)
    gestionnaireErreur("Erreur de connexion");
  puts("Connexion au serveur réussie");
  return socketServeur;
}

void *entrerEtEnvoyerMessages(void *arg)
{
  argsThread *args = (argsThread *)arg;
  char message[args->tailleMessage];

  while (strcmp(message, "fin") != 0)
  {
    while (entrerMessage(message, args->tailleMessage) == -1)
      ;
    if (send(args->socket, message, args->tailleMessage, 0) < 0)
      gestionnaireErreur("Erreur lors de l'envoi du message");
  }

  puts("Vous avez quitté la conversation");
  exit(EXIT_SUCCESS);
}

void *recevoirEtAfficherMessages(void *arg)
{
  argsThread *args = (argsThread *)arg;
  char message[args->tailleMessage];
  int reponse = 1;

  while (1)
  {
    reponse = recv(args->socket, message, args->tailleMessage, 0);

    if (reponse < 0)
      gestionnaireErreur("Erreur lors de la reception du message");

    if (reponse == 0)
    {
      puts("⚠️  \033[31mUne erreur serveur est survenue, vous avez été déconnecté\033[0m.");
      exit(EXIT_FAILURE);
    }

    if (strcmp(message, "fin") == 0)
    {
      puts("Vous avez quitté la conversation");
      exit(EXIT_SUCCESS);
    }

    printf("Ami : %s\n", message);
  }

  exit(EXIT_FAILURE);
}

int entrerMessage(char *message, const int tailleMessage)
{
  if (fgets(message, tailleMessage, stdin) == NULL || message[0] == '\n')
  {
    printf("⚠️  \033[31mLe message ne peut pas être vide\033[0m\n");
    return -1;
  }

  if (strlen(message) == tailleMessage - 1 && message[tailleMessage - 2] != '\n')
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