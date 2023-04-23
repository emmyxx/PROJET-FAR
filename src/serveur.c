#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/serveur.h"

#define NB_ARGS_ATTENDUS 2
#define TAILLE_MESSAGE 256

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);
  const int port = atoi(argv[1]);
  const int socketEcouteur = creerSocketEcouteur(port, 2);

  client client1 = {
    nom : "client1",
    estConnecte : 0
  };

  client client2 = {
    nom : "client2",
    estConnecte : 0
  };

  char message[TAILLE_MESSAGE];

  while (1)
  {
    if (!client1.estConnecte)
    {
      client1.socket = accepterClient(socketEcouteur);
      client1.estConnecte = 1;
    }

    if (!client2.estConnecte)
    {
      client2.socket = accepterClient(socketEcouteur);
      client2.estConnecte = 1;
    }

    strcpy(message, "client1");
    send(client1.socket, message, TAILLE_MESSAGE, 0);
    strcpy(message, "client2");
    send(client2.socket, message, TAILLE_MESSAGE, 0);

    demarrerConversation(&client1, &client2, TAILLE_MESSAGE);
  }

  close(socketEcouteur);
  printf("Fin du programme\n");
}

int accepterClient(int socketEcouteur)
{
  struct sockaddr_in adresseClient;
  socklen_t longueurAdrClient = sizeof(struct sockaddr_in);
  int socketClient = accept(socketEcouteur, (struct sockaddr *)&adresseClient, &longueurAdrClient);

  if (socketClient < 0)
    gestionnaireErreur("Erreur lors de l'acceptation d'un client");

  printf("Client Connecté\n");

  return socketClient;
}

void gestionnaireArguments(int argc, char *argv[])
{
  if (argc != NB_ARGS_ATTENDUS)
  {
    fprintf(stderr,
            "Erreur : %d arguments attendus, mais %d ont été fournis.\n",
            NB_ARGS_ATTENDUS, argc);
    fprintf(stderr, "Utilisation : %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

int creerSocketEcouteur(int port, int nbClientsEnAttente)
{
  int socketEcouteur = socket(PF_INET, SOCK_STREAM, 0);

  if (socketEcouteur < 0)
    gestionnaireErreur("Erreur de création de la socket");

  printf("Socket Créé\n");

  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  adresseEcouteur.sin_addr.s_addr = INADDR_ANY;
  adresseEcouteur.sin_port = htons(port);

  if (bind(socketEcouteur, (struct sockaddr *)&adresseEcouteur, sizeof(adresseEcouteur)) < 0)
    gestionnaireErreur("Erreur de nommage de la socket");

  printf("Socket Nommé\n");

  if (listen(socketEcouteur, nbClientsEnAttente) < 0)
    gestionnaireErreur("Erreur de passage en mode écoute");

  printf("Mode écoute\n");

  return socketEcouteur;
}

int demarrerConversation(client *emetteur, client *recepteur, const int tailleMessage)
{
  char message[tailleMessage];

  printf("Début de la conversation entre %s et %s\n", emetteur->nom, recepteur->nom);

  while (strcmp(message, "fin") != 0)
  {
    if (recv(emetteur->socket, message, tailleMessage, 0) == 0 ||
        strcmp(message, "fin") == 0)
    {
      close(emetteur->socket);
      emetteur->estConnecte = 0;
      printf("%s s'est déconnecté\n", emetteur->nom);
      send(recepteur->socket, "fin", tailleMessage, 0);
      break;
    }

    printf("%s : %s\n", emetteur->nom, message);

    send(recepteur->socket, message, tailleMessage, 0);

    // Inversion des clients émetteur et recepteur
    client *temp = emetteur;
    emetteur = recepteur;
    recepteur = temp;
  }

  printf("Fin de la conversation\n");
  return 0;
}