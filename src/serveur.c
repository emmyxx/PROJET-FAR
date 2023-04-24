#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/common.h"
#include "../include/serveur.h"

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);
  const int port = atoi(argv[1]);
  const int socketEcouteur = creerSocketEcouteur(port, NB_CLIENTS_EN_ATTENTE);
  client *clients[NB_CLIENTS_MAX];
  for (int i = 0; i < NB_CLIENTS_MAX; i++)
    clients[i] = NULL;

  while (1)
  {
    // Accepte des clients tant que l'on n'a pas atteint la limite
    if (avoirNombreClientsConnectes(clients) < NB_CLIENTS_MAX)
    {
      client *nouveauClient = (client *)malloc(sizeof(client));

      // Génère le nom du client
      char *nomClient = NULL;
      genererNomClient(&nomClient);
      nouveauClient->nom = nomClient;

      // Attend qu'un client se connecte
      nouveauClient->socket = accepterClient(socketEcouteur);
      if (nouveauClient->socket < 0)
        gestionnaireErreur("Erreur d'acceptation du client");
      printf("Client %s connecté\n", nouveauClient->nom);

      ajouterAuTableau(clients, nouveauClient);

      // Démarre le thread de broadcast, qui va recevoir le message du client et l'envoyer à tous les autres
      pthread_t threadBroadcast;
      argsThread *args = (argsThread *)malloc(sizeof(argsThread));
      args->client = nouveauClient;
      args->clients = clients;
      pthread_create(&threadBroadcast, NULL, broadcast, args);
    }
  }

  close(socketEcouteur);
  printf("Fin du programme\n");
}

int accepterClient(const int socketEcouteur)
{
  struct sockaddr_in adresseClient;
  socklen_t longueurAdrClient = sizeof(struct sockaddr_in);
  int socketClient = accept(socketEcouteur, (struct sockaddr *)&adresseClient, &longueurAdrClient);
  return socketClient < 0 ? -1 : socketClient;
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

  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  adresseEcouteur.sin_addr.s_addr = INADDR_ANY;
  adresseEcouteur.sin_port = htons(port);

  if (bind(socketEcouteur, (struct sockaddr *)&adresseEcouteur, sizeof(adresseEcouteur)) < 0)
    gestionnaireErreur("Erreur de nommage de la socket");

  if (listen(socketEcouteur, nbClientsEnAttente) < 0)
    gestionnaireErreur("Erreur de passage en mode écoute");

  return socketEcouteur;
}

void *broadcast(void *arg)
{
  argsThread *args = (argsThread *)arg;
  char message[TAILLE_MESSAGE];
  int reponse;
  while (1)
  {
    reponse = recv(args->client->socket, message, TAILLE_MESSAGE, 0);

    if (reponse < 0)
      gestionnaireErreur("Erreur de réception du message"); // FIXME ne pas faire crasher le programme

    if (reponse == 0 || strcmp(message, "fin") == 0)
    {
      break;
    }

    printf("%s : %s\n", args->client->nom, message);

    // Coupe la taille de "message" de façon à ce qu'elle ne dépasse pas 255 caractères
    // quand on la concatène avec le nom du client
    char messageAvecNom[TAILLE_MESSAGE];
    snprintf(messageAvecNom, TAILLE_MESSAGE - 1, "%s : %.*s", args->client->nom, TAILLE_MESSAGE - (int)strlen(args->client->nom), message);

    for (int i = 0; i < NB_CLIENTS_MAX; i++)
    {
      if (args->clients[i] != args->client && args->clients[i] != NULL)
      {
        if (send(args->clients[i]->socket, messageAvecNom, TAILLE_MESSAGE, 0) < 0)
          gestionnaireErreur("Erreur d'envoi du message"); // FIXME ne pas faire crasher le programme
      }
    }
  }

  printf("Fin de la connexion avec le client %s\n", args->client->nom);
  close(args->client->socket);
  supprimerDuTableau(args->clients, args->client);
  pthread_exit(NULL);
}

int genererNomClient(char **nomClient)
{
  *nomClient = (char *)malloc(sizeof(char) * 11);
  if (*nomClient == NULL)
    return -1;
  int nombreAleatoire = 1 + rand() % (1000 - 1 + 1);
  sprintf(*nomClient, "client%d", nombreAleatoire);
  return 0;
}

int avoirNombreClientsConnectes(client **clients)
{
  int nbClientsConnectes = 0;
  for (int i = 0; i < NB_CLIENTS_MAX; i++)
  {
    if (clients[i] != 0)
    {
      nbClientsConnectes++;
    }
  }
  return nbClientsConnectes;
}

int ajouterAuTableau(client **clients, client *clientAAjouter)
{
  for (int i = 0; i < NB_CLIENTS_MAX; i++)
  {
    if (clients[i] == NULL)
    {
      clients[i] = clientAAjouter;
      return 0;
    }
  }
  return -1;
}

int supprimerDuTableau(client **clients, client *clientASupprimer)
{
  for (int i = 0; i < NB_CLIENTS_MAX; i++)
  {
    if (clients[i] == clientASupprimer)
    {
      free(clients[i]->nom);
      free(clients[i]);
      clients[i] = NULL;
      return 0;
    }
  }
  return -1;
}