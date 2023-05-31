#include <stdlib.h>
#include <sys/stat.h>

#include "../typesMessage.h"

#define NB_ARGS_ATTENDUS 2
#define CHEMIN_DOSSIER_FICHIERS_LOCAUX "./repertoire_client/"
#define IP_SERVEUR "127.0.0.1"

/* -------------------------------------------------------------------------- */
/*                           Divers                                           */
/* -------------------------------------------------------------------------- */
void gestionnaireArguments(int argc, char *argv[]);
int creerConnexionServeur(const char *ipServeur, const int portServeur);
void gestionnaireSignal(int signum);
void arreterCommunication();
int afficherManuel();

/* -------------------------------------------------------------------------- */
/*                      Envoi et réception des fichiers                       */
/* -------------------------------------------------------------------------- */

typedef struct {
  int socketServeur;
  MorceauFichier morceauFichier;
} argsThreadFichier;

/**
 * @brief Retourne les informations des fichiers (et non pas dossiers) présents dans le dossier
 * passé en paramètre.
 * @param taille Un pointeur vers une variable qui contiendra la taille du
 * tableau retourné.
 * @param cheminDossier Le chemin du dossier dont on veut récupérer les
 * informations des fichiers.
 * @return Un tableau d'informations sur uniquement les fichiers (et non pas les dossiers) du
 * chemin du dossier passé en paramètres, ou NULL si une erreur s'est produite.
 * @note Il faut appeler free() sur le tableau retourné pour libérer la mémoire
 */
struct dirent *recupererTableauFichiers(size_t *taille, const char *cheminDossier);

/**
 * @brief Affiche les informations des fichiers du chemin passé en paramètres.
 * @param cheminDossier Le chemin du dossier dont on veut afficher ses fichiers.
 * @return 0 si l'affichage s'est bien passé, -1 si une erreur s'est produite.
 */
int afficherFichiers(const char *cheminDossier);

int envoyerFichier(FILE *pointeurFichier, int socketServeur, MorceauFichier morceauFichier);

/* -------------------------------------------------------------------------- */
/*                    Réception et traitement des messages                    */
/* -------------------------------------------------------------------------- */

void *threadReceptionMessages(void *arg);
int routageMessageRecu(void *messageRecu);
int recevoirMessageBroadcast(const MessageBroadcast messageBroadcast);
int afficherMessageAlerte(char *message, TypeAlerte typeAlerte);
int recevoirMessagePrive(const MessagePrive messagePrive);

/* -------------------------------------------------------------------------- */
/*                  Saisie, formattage et envoi des messages                  */
/* -------------------------------------------------------------------------- */

void *threadEnvoiMessages(void *arg);

/**
 * @brief Lit une chaîne de caractères à partir de l'entrée standard.
 * @param message La chaîne de caractères lue depuis l'entrée standard.
 * @param tailleMessage La taille du tableau.
 * @return 0 si la lecture de la chaîne est réussie, -1 si aucun caractère
 * n'est entré ou si l'utilisateur a entré plus de tailleMessage - 1 caractères.
 */
int entrerMessage(char *message, const int tailleMessage);

int routageEnvoiMessage(const char *saisie, const int socketServeur);
void nettoyerBufferEntree();

/* -------------------------------------------------------------------------- */
/*                       Réception et envoi des fichiers                      */
/* -------------------------------------------------------------------------- */

typedef struct {
  int socketServeur;
  Fichier fichier;
} argsThreadEnvoiFichier;

void *threadEnvoiFichier(void *arg);
void *threadEnvoiFichier2(void *arg);
void *threadReceptionFichier(void *arg);