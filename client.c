#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int NB_ARGS_ATTENDUS = 3;

void gestionnaireErreur(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

  if (argc != NB_ARGS_ATTENDUS)
  {
    fprintf(stderr,
            "Erreur : %d arguments attendus, mais %d ont été fournis.\n",
            NB_ARGS_ATTENDUS, argc);
    fprintf(stderr, "Utilisation : %s <adresse_ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const int PORT_SERVEUR = atoi(argv[2]);
  const char* IP_SERVEUR = argv[1];

  printf("Début programme\n");
  
  // Création de la socket
  int socketEcouteur = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");

  if (socketEcouteur == -1) 
    gestionnaireErreur("Erreur de création de la socket");

  // Configuration de l'adresse du serveur
  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  // inet_pton convertit une chaîne de caractères représentant une adresse IP en une adresse réseau
  inet_pton(AF_INET, IP_SERVEUR, &(adresseEcouteur.sin_addr));
  adresseEcouteur.sin_port = htons(PORT_SERVEUR);
  socklen_t longueurAdresse = sizeof(struct sockaddr_in);

  // Connexion à la socket serveur
  if(connect(socketEcouteur, (struct sockaddr *) &adresseEcouteur, longueurAdresse) == -1) 
    gestionnaireErreur("Erreur de connexion");
  
  printf("Socket Connecté\n");

  // Préparation du message à envoyer au serveur
  char * messageDuClient = "TEST";
  
  // Envoi du message au serveur
  if(send(socketEcouteur, messageDuClient, strlen(messageDuClient), 0) == -1) 
    gestionnaireErreur("Erreur d'envoi");

  printf("Message Envoyé \n");

  // Réception de la réponse du serveur
  int reponseDuServeur;
  
  if(recv(socketEcouteur, &reponseDuServeur, sizeof(int), 0) == -1) 
    gestionnaireErreur("Erreur de réception");

  printf("Réponse reçue : %d\n", reponseDuServeur);

  // Fermeture de la socket
  close(socketEcouteur);
  printf("Fin du programme");

  return 0;
}
