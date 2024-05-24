build: src/main.c src/json.c src/string.c src/vector.c src/parsing.c
	gcc -o json src/main.c src/json.c src/string.c src/vector.c src/parsing.c

test: build
	./json test.json
