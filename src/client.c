#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NB_ARGS_ATTENDUS 4
#define TAILLE_MESSAGE 256

void gestionnaireErreur(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{

  if (argc != NB_ARGS_ATTENDUS)
  {
    fprintf(stderr,
            "Erreur : %d arguments attendus, mais %d ont été fournis.\n",
            NB_ARGS_ATTENDUS, argc);
    fprintf(stderr, "Utilisation : %s <adresse_ip> <port> <client1|client2>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *IP_SERVEUR = argv[1];
  const int PORT_SERVEUR = atoi(argv[2]);
  const int estClient1 = strcmp(argv[3], "client1") == 0;

  printf("Début programme\n");

  // Création de la socket
  int socketServeur = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");

  if (socketServeur == -1)
    gestionnaireErreur("Erreur de création de la socket");

  // Configuration de l'adresse du serveur
  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  // inet_pton convertit une chaîne de caractères représentant une adresse IP en une adresse réseau
  inet_pton(AF_INET, IP_SERVEUR, &(adresseEcouteur.sin_addr));
  adresseEcouteur.sin_port = htons(PORT_SERVEUR);
  socklen_t longueurAdresse = sizeof(struct sockaddr_in);

  // Connexion à la socket serveur
  if (connect(socketServeur, (struct sockaddr *)&adresseEcouteur, longueurAdresse) == -1)
    gestionnaireErreur("Erreur de connexion");

  printf("Socket Connecté\n");

  char messageEnvoye[TAILLE_MESSAGE];
  char messageRecu[TAILLE_MESSAGE];

  while (1)
  {
    // Envoie le message en premier
    if (estClient1)
    {
      printf("Moi: ");
      fgets(messageEnvoye, TAILLE_MESSAGE, stdin);
      printf("\n");

      strtok(messageEnvoye, "\n"); // remplacement du caractère de nouvelle ligne par un caractère nul

      send(socketServeur, messageEnvoye, TAILLE_MESSAGE, 0);

      if (recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) == 0)
        break;
    }

    // Attend que le correspondant ait envoyé son message
    else
    {
      if (recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) == 0)
        break;

      printf("Ami: %s\n", messageRecu);

      printf("Moi: ");
      fgets(messageEnvoye, TAILLE_MESSAGE, stdin);

      strtok(messageEnvoye, "\n"); // remplacement du caractère de nouvelle ligne par un caractère nul

      send(socketServeur, messageEnvoye, TAILLE_MESSAGE, 0);
    }
  }

  // Fermeture de la socket
  close(socketServeur);
  printf("Fin de la connexion\n");

  return 0;
}
