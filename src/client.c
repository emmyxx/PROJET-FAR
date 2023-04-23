#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/client.h"

#define NB_ARGS_ATTENDUS 3
#define TAILLE_MESSAGE 256

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);

  const char *ipServeur = argv[1];
  const int portServeur = atoi(argv[2]);

  char messageEnvoye[TAILLE_MESSAGE];
  char messageRecu[TAILLE_MESSAGE];

  const int socketServeur = creerConnexionServeur(ipServeur, portServeur);

  while (1)
  {
    // Réception du message indiquant le rôle du client
    if (recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) == 0)
      gestionnaireErreur("Erreur lors de la communication avec le serveur");

    const int estClient1 = strcmp(messageRecu, "client1") == 0 ? 1 : 0;

    printf("Début de la conversation\n");

    if (!estClient1)
    {
      if (recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) == 0)
        gestionnaireErreur("Erreur lors de la communication avec le serveur");
      if (strcmp(messageRecu, "fin") != 0)
        printf("Ami : %s\n", messageRecu);
    }

    while (strcmp(messageRecu, "fin") != 0)
    {
      while (entrerMessage(messageEnvoye) == -1)
        ;
      send(socketServeur, messageEnvoye, TAILLE_MESSAGE, 0);
      if (recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) == 0)
      {
        gestionnaireErreur("Erreur lors de la communication avec le serveur");
      }
      if (strcmp(messageRecu, "fin") == 0)
      {
        break;
      }
      printf("Ami: %s\n", messageRecu);
    }

    printf("Fin de la conversation\n");
  }

  fermerSocketServeur(socketServeur);
  return 0;
}

void fermerSocketServeur(int socketServeur)
{
  close(socketServeur);
  printf("Socket fermée\n");
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
  // Création de la socket
  int socketServeur = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");

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

  printf("Socket Connecté\n");

  return socketServeur;
}

int entrerMessage(char *message)
{
  printf("Vous : ");

  if (fgets(message, TAILLE_MESSAGE, stdin) == NULL || message[0] == '\n')
  {
    printf("⚠️  \033[31mLe message ne peut pas être vide\033[0m\n");
    return -1;
  }

  if (strlen(message) == TAILLE_MESSAGE - 1 && message[TAILLE_MESSAGE - 2] != '\n')
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