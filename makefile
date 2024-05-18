build: src/main.c src/json.c src/string.c
	gcc -o json src/*.c

run: build
	./json
