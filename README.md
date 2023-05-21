# PROJET-FAR

### Sprint 1
 Mise en place d’un serveur qui puisse gérer n clients, pour cela mettre en place un tableau partagé pour stocker leurs identifiants sockets, par défaut, les messages arrivant depuis un client sont relayés à tous les autres présents. ✅

### Sprint 2
- Les clients peuvent avoir un pseudonyme ✅
- Les clients peuvent envoyer des messages privés ✅
- Les clients peuvent quitter une conversation ✅
- Les clients peuvent afficher les commandes disponibles ✅
- On doit utiliser un mutex sur le tableau des clients ✅

### Sprint 3
- Création répertoire de fichiers clients ✅
- Les clients peuvent choisir à travers le CLI un fichier a envoyer au serveur à partir d'un dossier local ✅
- Les clients peuvent choisir à travers le CLI un fichier à recevoir à partir d'un dossier dans le serveur
- Thread d'envoi : Les clients envoient nom + taille du fichier au serveur + envoie fichier partie par partie
- Thread reception : Les client recoivent un fichier du serveur
- Thread reception : Le serveur reçoit le fichier du client
- Thread d'envoi : serveur 