#ifndef CONSTANTES_H
#define CONSTANTES_H

// Le nombre maximum de caractères qu'un client peut écrire sur le chat.
#define TAILLE_SAISIE_CLIENT 1024

// La taille maximale des arguments d'un message formaté.
#define TAILLE_ENTETE 256

// La taille des messages envoyés en TCP.
#define TAILLE_MESSAGE_TCP TAILLE_SAISIE_CLIENT + TAILLE_ENTETE

// La taille maximale du pseudonyme d'un client.
#define TAILLE_PSEUDO 20

// Les différents niveaux d'importance des messages des messages 
// d'alerte envoyés par le serveur.
typedef enum
{
    INFORMATION,
    AVERTISSEMENT,
    ERREUR,
} TypeAlerte;

#endif
