#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/common.h"
#include "../include/client.h"

#define NB_ARGS_ATTENDUS 4
#define TAILLE_MESSAGE 256

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);

  const char *ipServeur = argv[1];
  const int portServeur = atoi(argv[2]);
  const int estClient1 = strcmp(argv[3], "client1") == 0;

  char messageEnvoye[TAILLE_MESSAGE];
  char messageRecu[TAILLE_MESSAGE];

  int socketServeur = creerConnexionServeur(ipServeur, portServeur);
  int estConnecte = 1;

  if (!estClient1)
  {
    if (recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) == 0)
      estConnecte = 0;
    else
      printf("Ami : %s\n", messageRecu);
  }

  while (estConnecte)
  {
    while (entrerMessage(messageEnvoye) == -1)
      ;
    send(socketServeur, messageEnvoye, TAILLE_MESSAGE, 0);
    estConnecte = recv(socketServeur, messageRecu, TAILLE_MESSAGE, 0) != 0; // FIXME si recv retourne une erreur, estConnecte sera à 1
    printf("Ami: %s\n", messageRecu);
  }

  pthread_t idthread;

  int thread = pthread_create(&idthread,NULL,//JSP);

  if(thread != 0){
    perror("Erreur de création du thread");
    exit(0);
  }

  //Ecoute du ctrl c
  signal(SIGINT, gestionSignal);

  pthread_join(idthread, NULL);

  fermerSocketServeur(socketServeur);
  return 0;
}

void fermerSocketServeur(int socketServeur)
{
  close(socketServeur);
  printf("Socket fermée\n");
}

void gestionnaireArguments(int argc, char *argv[])
{
  if (argc != NB_ARGS_ATTENDUS)
  {
    fprintf(stderr,
            "Erreur : %d arguments attendus, mais %d ont été fournis.\n",
            NB_ARGS_ATTENDUS, argc);
    fprintf(stderr, "Utilisation : %s <adresse_ip> <port> <client1|client2>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

int creerConnexionServeur(const char *ipServeur, const int portServeur)
{
  // Création de la socket
  int socketServeur = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");

  if (socketServeur == -1)
    gestionnaireErreur("Erreur de création de la socket");

  // Configuration de l'adresse du serveur
  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  // inet_pton convertit une chaîne de caractères représentant une adresse IP en une adresse réseau
  inet_pton(AF_INET, ipServeur, &(adresseEcouteur.sin_addr));
  adresseEcouteur.sin_port = htons(portServeur);
  socklen_t longueurAdresse = sizeof(struct sockaddr_in);

  // Connexion à la socket serveur
  if (connect(socketServeur, (struct sockaddr *)&adresseEcouteur, longueurAdresse) == -1)
    gestionnaireErreur("Erreur de connexion");

  printf("Socket Connecté\n");

  return socketServeur;
}

int entrerMessage(char *message)
{
  printf("Vous : ");

  if (fgets(message, TAILLE_MESSAGE, stdin) == NULL || message[0] == '\n')
  {
    printf("⚠️  \033[31mLe message ne peut pas être vide\033[0m\n");
    return -1;
  }

  if (strlen(message) == TAILLE_MESSAGE - 1 && message[TAILLE_MESSAGE - 2] != '\n')
  {
    printf("⚠️  \033[31mLe message est trop long\033[0m\n");
    nettoyerBufferEntree();
    return -1;
  }

  // Remplacement du caractère de nouvelle ligne par un caractère nul
  strtok(message, "\n");

  return 0;
}

// Si l'utilisateur entre plus de caractères que la taille du message, les caractères supplémentaires sont stockés dans le buffer d'entrée,
// ce qui peut poser problème pour les prochains appels à fgets
void nettoyerBufferEntree()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void gestionSignal(int sig){
  envoiMessage(socketServeur, "fin\n");

  printf("fin du programme\n");
  exit(0);
}

int envoiMessage(int socket, char* msg){
  int res = 0;
  int len = strlen(msg) + 1;
  char* message = (char*)malloc(len*sizeof(char));
  strcpy(message,msg);

  int envLen = send(socket, &len, sizeof(len),0);
  if(envLen == -1){
    perror("Erreur envoi taille message");
    res = -1;
  }
  else {
    int envoiMsg = send(socket, message, len, 0);
    if(envoiMsg == -1){
      perror("Erreur envoi message");
      res = -1
    }
    else printf("Message envoyé: \n%s\n"n message);
  }
  free(message);
  return res;
}