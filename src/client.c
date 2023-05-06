#include "../include/common.h"
#include "../include/client.h"

int main(int argc, char *argv[])
{
  gestionnaireArguments(argc, argv);

  const char *ipServeur = argv[1];
  const int portServeur = atoi(argv[2]);
  int socketServeur = creerConnexionServeur(ipServeur, portServeur);

  pthread_t threadRecevoir, threadEnvoyer;
  if (pthread_create(&threadEnvoyer, NULL, entrerEtEnvoyerMessages, &socketServeur) != 0)
    gestionnaireErreur("Erreur lors de la création du thread d'envoi");

  if (pthread_create(&threadRecevoir, NULL, recevoirEtAfficherMessages, &socketServeur) != 0)
    gestionnaireErreur("Erreur lors de la création du thread de réception");

  pthread_join(threadEnvoyer, NULL);
  pthread_join(threadRecevoir, NULL);

  close(socketServeur);
  return 0;
}

void gestionnaireArguments(int argc, char *argv[])
{
  if (argc != NB_ARGS_ATTENDUS)
  {
    fprintf(stderr,
            "Erreur : %d arguments attendus, mais %d ont été fournis.\n",
            NB_ARGS_ATTENDUS, argc);
    fprintf(stderr, "Utilisation : %s <adresse_ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

int creerConnexionServeur(const char *ipServeur, const int portServeur)
{
  int socketServeur = socket(PF_INET, SOCK_STREAM, 0);

  if (socketServeur == -1)
    gestionnaireErreur("Erreur de création de la socket");

  // Configuration de l'adresse du serveur
  struct sockaddr_in adresseEcouteur;
  adresseEcouteur.sin_family = AF_INET;
  inet_pton(AF_INET, ipServeur, &(adresseEcouteur.sin_addr));
  adresseEcouteur.sin_port = htons(portServeur);
  socklen_t longueurAdresse = sizeof(struct sockaddr_in);

  // Connexion à la socket serveur
  if (connect(socketServeur, (struct sockaddr *)&adresseEcouteur, longueurAdresse) == -1)
    gestionnaireErreur("Erreur de connexion");

  return socketServeur;
}

void *entrerEtEnvoyerMessages(void *arg)
{
  int socketServeur = *(int *)arg;
  char message[TAILLE_MESSAGE_TCP];

  while (strcmp(message, "fin") != 0)
  {
    while (entrerMessage(message, TAILLE_SAISIE_CLIENT) == -1)
      ;
    if (send(socketServeur, message, TAILLE_MESSAGE_TCP, 0) < 0)
      gestionnaireErreur("Erreur lors de l'envoi du message");
  }

  puts("Vous avez quitté la conversation");
  exit(EXIT_SUCCESS);
}

void *recevoirEtAfficherMessages(void *arg)
{
  int socketServeur = *(int *)arg;
  char message[TAILLE_MESSAGE_TCP];
  int reponse = 1;

  while (true)
  {
    reponse = recv(socketServeur, message, TAILLE_MESSAGE_TCP, 0);

    if (reponse < 0)
      gestionnaireErreur("Erreur lors de la reception du message");

    if (reponse == 0)
    {
      puts("⚠️  \033[31mUne erreur serveur est survenue, vous avez été déconnecté.\033[0m");
      exit(EXIT_FAILURE);
    }

    printf("%s\n", message);
  }

  exit(EXIT_FAILURE);
}

int entrerMessage(char *message, const int tailleMessage)
{
  if (fgets(message, tailleMessage, stdin) == NULL || message[0] == '\n')
  {
    // TODO standardiser messages d'erreurs en utilisant une fonction dédiée
    printf("⚠️  \033[31mLe message ne peut pas être vide\033[0m\n");
    return -1;
  }

  if (strlen(message) == tailleMessage - 1 && message[tailleMessage - 2] != '\n')
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

char *formaterSaisieClient(char *messageTCP, const char *saisie)
{
  // Remplit messageTCP de '\0' (au cas où il y aurait des caractères résiduels)
  memset(messageTCP, '\0', TAILLE_MESSAGE_TCP);

  char *copieSaisie = strdup(saisie);

  const char *nomCommande = strtok(copieSaisie, " ");

  if (nomCommande == NULL)
  {
    errno = EINVAL;
    free(copieSaisie);
    return NULL;
  }

  // Empêche le programme de planter lors de la création de saisieSansCommande
  // si l'utilisateur entre une commande trop longue
  if (strlen(nomCommande) >= TAILLE_SAISIE_CLIENT - 1)
  {
    errno = EINVAL;
    free(copieSaisie);
    return NULL;
  }

  // "/mp pseudo message" -> "pseudo message"
  char *saisieSansCommande = copieSaisie + strlen(nomCommande) + 2;

  // S'il n'y a pas de commande
  if (saisie[0] != '/')
    messageTCP = formaterEnBroadcast(messageTCP, saisieSansCommande);

  else if (strcmp(nomCommande, "/pseudo") == 0)
    messageTCP = formaterEnPseudo(messageTCP, saisieSansCommande);

  // Si la saisie du client ne respecte pas la syntaxe d'une commande
  if (messageTCP == NULL || messageTCP[0] == '\0')
  {
    errno = EINVAL;
    free(copieSaisie);
    return NULL;
  }

  // DEBUG
  printf("messageTCP : %s\n", messageTCP);

  return messageTCP;
}

char *formaterEnBroadcast(char *messageTCP, char *saisieClient)
{
  strcat(messageTCP, "MESSAGE_BROADCAST ");
  strcat(messageTCP, saisieClient);
  return messageTCP;
}

char *formaterEnPseudo(char *messageTCP, char *saisieClient)
{
  const char *pseudo = strtok(saisieClient, " ");
  if (pseudo == NULL)
  {
    errno = EINVAL;
    return NULL;
  }

  strcat(messageTCP, "PSEUDO ");
  strcat(messageTCP, pseudo);

  return messageTCP;
}
