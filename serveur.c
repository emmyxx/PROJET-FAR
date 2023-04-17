#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int NB_ARGS_ATTENDUS = 2;
const int TAILLE_MESSAGE = 20;

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
    fprintf(stderr, "Utilisation : %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int PORT = atoi(argv[1]);

  int socketEcouteur = socket(PF_INET, SOCK_STREAM, 0);
  if (socketEcouteur < 0)
    gestionnaireErreur("Erreur de création de la socket");

  printf("Socket Créé\n");

  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  adresseEcouteur.sin_addr.s_addr = INADDR_ANY;
  adresseEcouteur.sin_port = htons(PORT);

  if (bind(socketEcouteur, (struct sockaddr *)&adresseEcouteur, sizeof(adresseEcouteur)) < 0)
    gestionnaireErreur("Erreur de nommage de la socket");

  printf("Socket Nommé\n");

  if (listen(socketEcouteur, 7) < 0)
    gestionnaireErreur("Erreur de passage en mode écoute");

  printf("Mode écoute\n");

  struct sockaddr_in adresseClient;
  socklen_t longueurAdrClient = sizeof(struct sockaddr_in);
  int socketClient = accept(socketEcouteur, (struct sockaddr *)&adresseClient, &longueurAdrClient);

  if (socketClient < 0)
    gestionnaireErreur("Erreur de connexion");

  printf("Client Connecté\n");

  char messageDuClient[TAILLE_MESSAGE];
  if (recv(socketClient, messageDuClient, sizeof(messageDuClient), 0) < 0)
    gestionnaireErreur("Erreur de reception ");

  printf("message reçu : %s\n", messageDuClient);

  int reponseDuServeur = 10;

  if (send(socketClient, &reponseDuServeur, sizeof(int), 0) < 0)
    gestionnaireErreur("Erreur d'envoie");

  printf("message Envoyé\n");

  close(socketClient);
  close(socketEcouteur);
  printf("Fin du programme\n");

  return 0;
}
