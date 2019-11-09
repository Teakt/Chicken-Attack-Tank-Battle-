CC=gcc
CFLAGS=-Wall
EXEC=ChickenATTACK

Lancement : $(EXEC) Execution clean

fichier.o : fichier.c fichier.h constantes.h

jeu.o : jeu.c jeu.h librairies.h fichier.h
	$(CC) -c $(CFLAGS) jeu.c

main.o : main.c librairies.h constantes.h jeu.h
	$(CC) -c $(CFLAGS) main.c

$(EXEC) : jeu.o main.o fichier.o fichier.o
	$(CC) -o $@ $^ $(LDFLAGS) -lSDL -lSDL_image -lSDL_mixer -g

Execution :
	./$(EXEC)

clean :
	rm -rf *.o
