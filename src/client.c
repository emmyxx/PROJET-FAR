#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/client.h"

#define NB_ARGS_ATTENDUS 4
#define TAILLE_MESSAGE 256

void fermerSocketServeur(int socketServeur);

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);

  const char *ipServeur = argv[1];
  const int portServeur = atoi(argv[2]);
  const int estClient1 = strcmp(argv[3], "client1") == 0;

  char messageEnvoye[TAILLE_MESSAGE];
  char messageRecu[TAILLE_MESSAGE];

  int socketServeur = creerConnexionServeur(ipServeur, portServeur);
  int estConnecte = 1;

  if (!estClient1)
  {
    if (recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) == 0)
      estConnecte = 0;
    else
      printf("Ami: %s\n", messageRecu);
  }

  while (estConnecte)
  {
    printf("Moi: ");

    fgets(messageEnvoye, TAILLE_MESSAGE, stdin);
    strtok(messageEnvoye, "\n"); // remplacement du caractère de nouvelle ligne par un caractère nul

    printf("\n");

    send(socketServeur, messageEnvoye, TAILLE_MESSAGE, 0);
    estConnecte = recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) != 0;
    
    printf("Ami: %s\n", messageRecu);
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
    fprintf(stderr, "Utilisation : %s <adresse_ip> <port> <client1|client2>\n", argv[0]);
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