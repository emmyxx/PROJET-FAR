#include <stdlib.h>
#include <stdbool.h>

#include "../include/constantes.h"
#include "../include/serveur/gestionnaireClients.h"
#include "../include/serveur/serveur.h"

client **creerTableauClients()
{
    client **listeClients = (client **)malloc(sizeof(client *) * NB_CLIENTS_MAX);
    for (int i = 0; i < NB_CLIENTS_MAX; i++)
        listeClients[i] = NULL;
    return (client **)malloc(sizeof(client *) * NB_CLIENTS_MAX);
}

void detruireTableauClients(client **listeClients)
{
    for (int i = 0; i < NB_CLIENTS_MAX; i++)
    {
        if (listeClients[i] != NULL)
        {
            free(listeClients[i]);
        }
    }
    free(listeClients);
}

int avoirNombreClientsConnectes(const client **listeClients)
{
    int nbClientsConnectes = 0;
    for (int i = 0; i < NB_CLIENTS_MAX; i++)
    {
        if (listeClients[i] != 0)
        {
            nbClientsConnectes++;
        }
    }
    return nbClientsConnectes;
}

int ajouterClient(client **listeClients, client *client)
{
    for (int i = 0; i < NB_CLIENTS_MAX; i++)
    {
        if (listeClients[i] == NULL)
        {
            listeClients[i] = client;
            return 0;
        }
    }
    return -1;
}

int supprimerClient(client **listeClients, client *client)
{
    for (int i = 0; i < NB_CLIENTS_MAX; i++)
    {
        if (listeClients[i] == client)
        {
            free(listeClients[i]);
            listeClients[i] = NULL;
            return 0;
        }
    }
    return -1;
}
