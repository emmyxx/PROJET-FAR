all: 
	gcc -g -Wall -o ./include/client ./src/client.c
	gcc -g -Wall -o ./include/serveur ./src/serveur.c

clean: 
	$(RM) ./include/client
	$(RM) ./include/serveur