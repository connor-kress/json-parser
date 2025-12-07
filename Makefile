GCC_FLAGS = -Wall -Wextra -std=c11

build: src/main.c src/json.c src/string.c src/vector.c src/parsing.c
	gcc $(GCC_FLAGS) -o json src/main.c src/json.c src/string.c src/vector.c src/parsing.c

test: build
	./json test.json
