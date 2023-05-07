#include "../include/formater_saisie_client.h"

#define NB_ARGS_ATTENDUS 4


/* -------------------------------------------------------------------------- */
/*                           Mise en place du client                          */
/* -------------------------------------------------------------------------- */
int creerConnexionServeur(const char *ipServeur, const int portServeur);


/* -------------------------------------------------------------------------- */
/*                    Réception et traitement des messages                    */
/* -------------------------------------------------------------------------- */
void *receptionMessages(void *arg);
int routageMessageRecu(void *messageRecu);
int recevoirMessageBroadcast(const MessageBroadcast messageBroadcast);
int afficherMessageAlerte(char *message, TypeAlerte typeAlerte);


/* -------------------------------------------------------------------------- */
/*                  Saisie, formattage et envoi des messages                  */
/* -------------------------------------------------------------------------- */
void *envoiMessages(void *arg);
/**
 * @brief Lit une chaîne de caractères à partir de l'entrée standard.
 * @param message La chaîne de caractères lue depuis l'entrée standard.
 * @param tailleMessage La taille du tableau.
 * @return 0 si la lecture de la chaîne est réussie, -1 si aucun caractère
 * n'est entré ou si l'utilisateur a entré plus de tailleMessage - 1 caractères.
 */
int entrerMessage(char *message, const int tailleMessage);
void nettoyerBufferEntree();
