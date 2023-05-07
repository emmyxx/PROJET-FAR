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

  puts("Serveur démarré");

  while (true)
  {
    // Accepte des clients tant que l'on n'a pas atteint la limite
    if (avoirNombreClientsConnectes(clients) < NB_CLIENTS_MAX)
    {
      client *nouveauClient = (client *)malloc(sizeof(client));

      // Génère le nom du client
      // char *nomClient = NULL;
      // genererNomClient(&nomClient);
      // strcpy(nouveauClient->nom, nomClient);

      // Attend qu'un client se connecte
      nouveauClient->socket = accepterClient(socketEcouteur);
      if (nouveauClient->socket < 0)
        gestionnaireErreur("Erreur d'acceptation du client");
      nouveauClient->estConnecte = true;

      ajouterAuTableau(clients, nouveauClient);

      pthread_t threadReception;
      argsThread *args = (argsThread *)malloc(sizeof(argsThread));
      args->client = nouveauClient;
      args->clients = clients;
      pthread_create(&threadReception, NULL, receptionMessages, args);
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

void *receptionMessages(void *arg)
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
    controlleurConnexion((const client **)listeClients, clientCourant, message);

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

  printf("Fin de la connexion avec le client %s\n", args->client->nom);
  close(args->client->socket);
  supprimerDuTableau(args->clients, args->client);
  pthread_exit(NULL);
}

int routageMessageRecu(client **listeClients, client *clientCourant, void *message)
{
  const TypeMessage typeMessage = *(TypeMessage *)message;

  if (typeMessage == MESSAGE_BROADCAST)
  {
    MessageBroadcast messageBroadcast = *(MessageBroadcast *)message;
    return controlleurMessageBroadcast((const client **)listeClients, (const client *)clientCourant, messageBroadcast);
  }

  if (typeMessage == PSEUDO)
  {
    AttributionPseudo pseudo = *(AttributionPseudo *)message;
    return controlleurAttributionPseudo((const client **)listeClients, clientCourant, pseudo);
  }

  return -1;
}

int controlleurConnexion(const client **listeClients, client *clientCourant, void *message)
{
  const TypeMessage typeMessage = *(TypeMessage *)message;
  if (typeMessage != PSEUDO)
  {
    envoyerMessageAlerte(clientCourant, "Le message TCP d'attritution du pseudonyme est mal formaté.", ERREUR);
    clientCourant->estConnecte = false;
    return -1;
  }

  AttributionPseudo pseudo = *(AttributionPseudo *)message;

  if (!estPseudoValide(listeClients, pseudo.pseudo))
  {
    envoyerMessageAlerte(clientCourant, "Ce pseudonyme est déjà utilisé.", ERREUR);
    clientCourant->estConnecte = false;
    return -1;
  }

  // Créer un string qui contient le message de bienvenue
  char messageBienvenue[TAILLE_MESSAGE_TCP];
  sprintf(messageBienvenue, "Bienvenue %s !", pseudo.pseudo);
  envoyerMessageAlerte(clientCourant, messageBienvenue, INFORMATION);
  // TODO broadcast de l'arrivée du client
  strcpy(clientCourant->nom, pseudo.pseudo);
  return 0;
}

int controlleurMessageBroadcast(const client **listeClients, const client *clientCourant, MessageBroadcast messageBroadcast)
{
  strcpy(messageBroadcast.expediteur, clientCourant->nom);
  broadcast(listeClients, clientCourant, &messageBroadcast);
  printf("%s broadcast : \"%s\"\n", clientCourant->nom, messageBroadcast.message);
  return 0;
}

int controlleurAttributionPseudo(const client **listeClients, client *clientCourant, AttributionPseudo pseudo)
{
  if (!estPseudoValide(listeClients, pseudo.pseudo))
  {
    envoyerMessageAlerte(clientCourant, "Ce pseudonyme est déjà utilisé.", AVERTISSEMENT);
    return -1;
  }

  strcpy(clientCourant->nom, pseudo.pseudo);
  return 0;
}

bool estPseudoValide(const client **listeClients, const char *pseudo)
{
  for (int i = 0; i < NB_CLIENTS_MAX; i++)
  {
    if (listeClients[i] != NULL && strcmp(listeClients[i]->nom, pseudo) == 0)
    {
      return false;
    }
  }

  return true;
}

int envoyerMessageAlerte(const client *clientCourant, char *message, TypeAlerte typeAlerte)
{
  MessageAlerte messageAlerte;
  messageAlerte.typeMessage = MESSAGE_ALERTE;
  messageAlerte.typeAlerte = typeAlerte;
  strcpy(messageAlerte.message, message);
  if (send(clientCourant->socket, &messageAlerte, TAILLE_MESSAGE_TCP, 0) < 0)
    return -1;
  return 0;
}

int broadcast(const client **listeClients, const client *clientCourant, const void *message)
{
  for (int i = 0; i < NB_CLIENTS_MAX; i++)
  {
    if (listeClients[i] != NULL && listeClients[i] != clientCourant)
    {
      if (send(listeClients[i]->socket, message, TAILLE_MESSAGE_TCP, 0) < 0)
        return -1;
    }
  }

  return 0;
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
      free(clients[i]);
      clients[i] = NULL;
      return 0;
    }
  }
  return -1;
}
