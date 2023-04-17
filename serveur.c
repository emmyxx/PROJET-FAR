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
    fprintf(stderr, "Utilisation : %s <adresse_IP> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Création de la socket
  // PF_INET : protocole IP
  // SOCK_STREAM : protocole TCP
  int dS = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");

  if (dS == -1)
  {
    perror("Erreur de création de la socket");
    exit(EXIT_FAILURE);
  }

  // Configuration de l'adresse
  // AF_INET : IPv4
  // INADDR_ANY : écoute sur toutes les interfaces réseaux
  struct sockaddr_in ad;
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  // hton convertir un entier en format réseau
  ad.sin_port = htons(atoi(argv[1]));

  // Associer l'adresse à la socket
  if (bind(dS, (struct sockaddr *)&ad, sizeof(ad)) == -1)
  {
    perror("Erreur de nommage de la socket");
    exit(EXIT_FAILURE);
  }

  printf("Socket Nommé\n");
  // Passer la socket en mode écoute
  if (listen(dS, 7) == -1)
  {
    perror("Erreur de passage en mode écoute");
    exit(EXIT_FAILURE);
  }

  printf("Mode écoute\n");

  // Accepter la connexion entrante
  struct sockaddr_in aC;
  socklen_t lg = sizeof(struct sockaddr_in);
  int dSC = accept(dS, (struct sockaddr *)&aC, &lg);
  int dSC = accept(dS, (struct sockaddr *)&aC, &lg);
  printf("Client Connecté\n");

  if (dSC == -1)
  {
    perror("Erreur de connexion");
    exit(EXIT_FAILURE);
  }

  // Recevoir le message du client
  char msg[20];
  if (recv(dSC, msg, sizeof(msg), 0) == -1)
  {
    perror("Erreur de reception ");
    exit(EXIT_FAILURE);
  }

  printf("Message reçu : %s\n", msg);


  // Préparer le message à envoyer au client
  int r = 10;


  // Envoyer le message au client
  if (send(dSC, &r, sizeof(int), 0) == -1)
  {
    perror("Erreur d'envoie");
    exit(EXIT_FAILURE);
  }

  printf("Message Envoyé\n");


  // Fermer la connexion avec le client et fermer la socket
  close(dSC);
  close(dS);
  printf("Fin du programme");
}
