all: main

main:
	g++ -I src/include -L src/lib -o run main.cpp back/gamestate.cpp back/hash.cpp back/tiles.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

clean:
	rm -f main
