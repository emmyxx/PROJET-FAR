#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  printf("Début programme\n");
  
  // Création de la socket
  int dS = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");

  // Configuration de l'adresse du serveur
  struct sockaddr_in aS;
  aS.sin_family = AF_INET;
  // inet_pton convertit une chaîne de caractères représentant une adresse IP en une adresse réseau
  inet_pton(AF_INET, argv[1], &(aS.sin_addr));
  aS.sin_port = htons(atoi(argv[2]));
  socklen_t lgA = sizeof(struct sockaddr_in);

  // Connexion à la socket serveur
  connect(dS, (struct sockaddr *) &aS, lgA);
  printf("Socket Connecté\n");

  // Préparation du message à envoyer au serveur
  char * m = "TEST";
  
  // Envoi du message au serveur
  send(dS, m, strlen(m), 0);
  printf("Message Envoyé \n");

  // Réception de la réponse du serveur
  int r;
  recv(dS, &r, sizeof(int), 0);
  printf("Réponse reçue : %d\n", r);

  // Fermeture de la socket
  shutdown(dS, 2);
  printf("Fin du programme");
}
