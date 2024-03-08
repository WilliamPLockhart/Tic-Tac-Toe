all: main

main:
	g++ -I src/include -L src/lib -o run main.cpp button.cpp words/hash.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

clean:
	rm -f main

