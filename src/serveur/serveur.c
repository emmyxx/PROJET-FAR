#include "../include/constantes.h"
#include "../include/common.h"
#include "../include/serveur/serveur.h"
#include "../include/serveur/gestionnaireClients.h"
#include "../include/serveur/routageMessageRecu.h"

pthread_mutex_t clients_mutex;

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);
  const int port = atoi(argv[1]);

  const int socketEcoute = creersocketEcoute(port);
  if (socketEcoute < 0)
  {
    perror("Erreur de création du socket écouteur");
    exit(EXIT_FAILURE);
  }

  client **clients = creerTableauClients(NB_CLIENTS_MAX);

  pthread_mutex_init(&clients_mutex, NULL);

  puts("Serveur démarré");

  while (true)
  {
    // Accepte des clients tant que l'on n'a pas atteint la limite
    if (avoirNombreClientsConnectes((const client **)clients) < NB_CLIENTS_MAX)
    {
      client *nouveauClient = (client *)malloc(sizeof(client));

      // Attend qu'un client se connecte
      nouveauClient->socket = accepterClient(socketEcoute);

      if (nouveauClient->socket < 0)
      {
        perror("Erreur d'acceptation d'un client");
        break;
      }

      nouveauClient->estConnecte = true;

      // TODO free ce malloc
      argsThread *args = (argsThread *)malloc(sizeof(argsThread));
      args->client = nouveauClient;
      args->clients = clients;

      pthread_mutex_lock(&clients_mutex);
      ajouterClient(clients, nouveauClient);
      pthread_mutex_unlock(&clients_mutex);

      pthread_t threadClient;
      pthread_create(&threadClient, NULL, threadTraitementMessagesClient, args);
    }
  }

  pthread_mutex_destroy(&clients_mutex);

  free(clients);
  close(socketEcoute);
  printf("Fin du programme\n");
  exit(EXIT_SUCCESS);
}

int accepterClient(const int socketEcoute)
{
  struct sockaddr_in adresseClient;
  socklen_t longueurAdrClient = sizeof(struct sockaddr_in);
  int socketClient = accept(socketEcoute, (struct sockaddr *)&adresseClient, &longueurAdrClient);
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

int creersocketEcoute(int port)
{
  int socketEcoute = socket(PF_INET, SOCK_STREAM, 0);

  if (socketEcoute < 0)
    return -1;

  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  adresseEcouteur.sin_addr.s_addr = INADDR_ANY;
  adresseEcouteur.sin_port = htons(port);

  if (bind(socketEcoute, (struct sockaddr *)&adresseEcouteur, sizeof(adresseEcouteur)) < 0)
    return -1;

  if (listen(socketEcoute, NB_CLIENTS_EN_ATTENTE) < 0)
    return -1;

  return socketEcoute;
}

void *threadTraitementMessagesClient(void *arg)
{
  argsThread *args = (argsThread *)arg;
  client **listeClients = args->clients;
  client *clientCourant = args->client;
  char message[TAILLE_MESSAGE_TCP];
  int reponse;

  // Attribution du pseudonyme
  reponse = recv(clientCourant->socket, message, TAILLE_MESSAGE_TCP, 0);
  if (reponse < 0)
    envoyerMessageAlerte(clientCourant, "Erreur serveur lors de la réception du pseudonyme.", ERREUR);
  else if (reponse == 0)
    clientCourant->estConnecte = false;
  else
    recevoirPseudo((const client **)listeClients, clientCourant, message);

  while (clientCourant->estConnecte)
  {
    reponse = recv(clientCourant->socket, message, TAILLE_MESSAGE_TCP, 0);
    if (reponse < 0)
      gestionnaireErreur("Erreur de réception du message"); // FIXME ne pas faire crasher le programme

    if (reponse == 0)
    {
      clientCourant->estConnecte = false;
      break;
    }

    routageMessageRecu(listeClients, clientCourant, message);
  }

  printf("%s s'est déconnecté(e).\n", args->client->nom);
  close(args->client->socket);

  pthread_mutex_lock(&clients_mutex);
  supprimerClient(args->clients, args->client);
  pthread_mutex_unlock(&clients_mutex);

  // TODO tuer le thread zombie
  pthread_exit(NULL);
}

int recevoirPseudo(const client **listeClients, client *clientCourant, void *message)
{
  const TypeMessage typeMessage = *(TypeMessage *)message;
  if (typeMessage != PSEUDO)
  {
    envoyerMessageAlerte(clientCourant, "Le message TCP d'attritution du pseudonyme est mal formaté.", ERREUR);
    clientCourant->estConnecte = false;
    return -1;
  }

  AttributionPseudo pseudo = *(AttributionPseudo *)message;

  if (pseudoExiste(listeClients, pseudo.pseudo))
  {
    envoyerMessageAlerte(clientCourant, "Ce pseudonyme est déjà utilisé.", ERREUR);
    clientCourant->estConnecte = false;
    return -1;
  }

  printf("%s s'est connecté(e).\n", pseudo.pseudo);
  char messageBienvenue[TAILLE_MESSAGE_TCP];
  sprintf(messageBienvenue, "Bienvenue %s !", pseudo.pseudo);
  envoyerMessageAlerte(clientCourant, messageBienvenue, INFORMATION);
  // TODO indiquer à tous les clients que le nouveau client s'est connecté
  strcpy(clientCourant->nom, pseudo.pseudo);
  return 0;
}
