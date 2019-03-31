# Makefile
install_audiolib:
	sudo apt-get install libsfml-dev

all: compila linka executa

compila:
	g++ tp2.cpp -c

linka:
	g++ tp2.o -o tp2 -lGL -lGLU -lglut -lSOIL -lsfml-audio

executa:
	./tp2