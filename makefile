all:	client2 serveur 

client2:	Client/client2.o
		gcc -o client2 Client/client2.o

serveur:	Serveur/awale.o Serveur/server2.o 
		gcc -o serveur Serveur/awale.o Serveur/server2.o

Client/client2.o:	Client/client2.c
		gcc -Wall -o Client/client2.o -c Client/client2.c

Serveur/server2.o:	Serveur/server2.c 
		gcc -Wall -o Serveur/server2.o -c Serveur/server2.c

Serveur/awale.o:	Serveur/awale.c 
		gcc -Wall -o  Serveur/awale.o -c Serveur/awale.c

clean:
		rm *.o