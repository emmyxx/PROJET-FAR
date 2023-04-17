#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

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

  // Configuration de l'adresse
  // AF_INET : IPv4
  // INADDR_ANY : écoute sur toutes les interfaces réseaux
  struct sockaddr_in ad;
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  // hton convertir un entier en format réseau
  ad.sin_port = htons(atoi(argv[1]));

  // Associer l'adresse à la socket
  bind(dS, (struct sockaddr *)&ad, sizeof(ad));
  printf("Socket Nommé\n");

  // Passer la socket en mode écoute
  listen(dS, 7);
  printf("Mode écoute\n");

  // Accepter la connexion entrante
  struct sockaddr_in aC;
  socklen_t lg = sizeof(struct sockaddr_in);
  int dSC = accept(dS, (struct sockaddr *)&aC, &lg);
  printf("Client Connecté\n");

  // Recevoir le message du client
  char msg[20];
  recv(dSC, msg, sizeof(msg), 0);
  printf("Message reçu : %s\n", msg);

  // Préparer le message à envoyer au client
  int r = 10;

  // Envoyer le message au client
  send(dSC, &r, sizeof(int), 0);
  printf("Message Envoyé\n");

  // Fermer la connexion avec le client et fermer la socket
  shutdown(dSC, 2);
  shutdown(dS, 2);
  printf("Fin du programme");
}
