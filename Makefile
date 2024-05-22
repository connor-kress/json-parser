build: src/main.c src/json.c src/string.c src/vector.c
	gcc -o json src/main.c src/json.c src/string.c src/vector.c

run: build
	./json
