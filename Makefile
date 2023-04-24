all:
	mkdir -p build
	gcc -g -Wall -pthread -o ./build/client ./src/client.c
	gcc -g -Wall -pthread -o ./build/serveur ./src/serveur.c

clean: 
	$(RM) ./build/client
	$(RM) ./build/serveur