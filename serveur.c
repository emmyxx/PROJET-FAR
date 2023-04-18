#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NB_ARGS_ATTENDUS 2
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

  int socketClient1 = accept(socketEcouteur, (struct sockaddr *)&adresseClient, &longueurAdrClient);

  if (socketClient1 < 0)
    gestionnaireErreur("Erreur de connexion");

  printf("Client1 Connecté\n");

  int socketClient2 = accept(socketEcouteur, (struct sockaddr *)&adresseClient, &longueurAdrClient);

  if (socketClient2 < 0)
    gestionnaireErreur("Erreur de connexion");

  printf("Client2 Connecté\n");

  char messageDuClient1[TAILLE_MESSAGE];
  char messageDuClient2[TAILLE_MESSAGE];

  while (strcmp(messageDuClient1, "fin") != 0 || strcmp(messageDuClient2, "fin") != 0)
  {
    // le serveur reçoit le message du client 1
    if (recv(socketClient1, messageDuClient1, TAILLE_MESSAGE, 0) == 0)
      break;

    printf("Le message reçu: %s\n", messageDuClient1);

    // le serveur renvoie le message du client 1 au client 2
    send(socketClient2, messageDuClient1, TAILLE_MESSAGE, 0);

    printf("Le message envoyé: %s\n", messageDuClient1);

    // le serveur reçoit le message du client 2
    if (recv(socketClient2, messageDuClient2, TAILLE_MESSAGE, 0) == 0)
      break;

    printf("Le message reçu: %s\n", messageDuClient2);

    // le serveur renvoie le message du client 2 au client 1
    send(socketClient1, messageDuClient2, TAILLE_MESSAGE, 0);

    printf("Le message envoyé: %s\n", messageDuClient2);
  }

  close(socketClient1);
  close(socketClient2);
  close(socketEcouteur);

  printf("Fin du programme\n");

  return 0;
}
