all:
	mkdir -p build
	gcc -g -Wall -o ./build/client ./src/client.c
	gcc -g -Wall -o ./build/serveur ./src/serveur.c

clean: 
	$(RM) ./build/client
	$(RM) ./build/serveur