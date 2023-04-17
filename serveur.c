#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  const int NB_ARGS_ATTENDUS = 3;

  if (argc != NB_ARGS_ATTENDUS)
  {
    fprintf(stderr,
            "Erreur : %d arguments attendus, mais %d ont été fournis.\n",
            NB_ARGS_ATTENDUS, argc);
    fprintf(stderr, "Utilisation : %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const int PORT = htons(atoi(argv[1]));

  // Création de la socket
  // PF_INET : protocole IP
  // SOCK_STREAM : protocole TCP
  int socketEcouteur = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");

  if (socketEcouteur == -1)
  {
    perror("Erreur de création de la socket");
    exit(EXIT_FAILURE);
  }

  // Configuration de l'adresse
  // AF_INET : IPv4
  // INADDR_ANY : écoute sur toutes les interfaces réseaux
  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  adresseEcouteur.sin_addr.s_addr = INADDR_ANY;
  adresseEcouteur.sin_port = PORT;

  // Associer l'adresse à la socket
  if (bind(socketEcouteur, (struct sockaddr *)&adresseEcouteur, sizeof(adresseEcouteur)) == -1)
  {
    perror("Erreur de nommage de la socket");
    exit(EXIT_FAILURE);
  }

  printf("Socket Nommé\n");

  // Passer la socket en mode écoute
  if (listen(socketEcouteur, 7) == -1)
  {
    perror("Erreur de passage en mode écoute");
    exit(EXIT_FAILURE);
  }

  printf("Mode écoute\n");

  // Accepter la connexion entrante
  struct sockaddr_in adresseClient;
  socklen_t longueurAdrClient = sizeof(struct sockaddr_in);
  int socketClient = accept(socketEcouteur, (struct sockaddr *)&adresseClient, &longueurAdrClient);
  printf("Client Connecté\n");

  if (socketClient == -1)
  {
    perror("Erreur de connexion");
    exit(EXIT_FAILURE);
  }

  // Recevoir le message du client
  char messageDuClient[20];
  if (recv(socketClient, messageDuClient, sizeof(messageDuClient), 0) == -1)
  {
    perror("Erreur de reception ");
    exit(EXIT_FAILURE);
  }

  printf("message reçu : %s\n", messageDuClient);


  // Préparer le message à envoyer au client
  int reponseDuServeur = 10;


  // Envoyer le message au client
  if (send(socketClient, &reponseDuServeur, sizeof(int), 0) == -1)
  {
    perror("Erreur d'envoie");
    exit(EXIT_FAILURE);
  }

  printf("message Envoyé\n");


  // Fermer la connexion avec le client et fermer la socket
  close(socketClient);
  close(socketEcouteur);
  printf("Fin du programme");
}
