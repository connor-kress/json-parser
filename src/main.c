#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "parsing.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect usage. Correct usage is as follows:\n");
        fprintf(stderr, "\t./json <PATH>\n");
        return 1;
    }
    const char *path = argv[1];
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File not found\n");
        return 1;
    }
    String json_str = new_string();
    char c;
    for (;;) {
        c = fgetc(file);
        if (c == EOF) break;
        push_char(&json_str, c);
    }
    fclose(file);
    // printf("Read: \"%s\"\n", json_str.data);
    bool parsing_err = false;
    Json json = parse_json(&json_str, &parsing_err);
    if (parsing_err) {
        free_json(json);
        return 1;
    }
    print_json(&json);
    putchar('\n');
    free_json(json);
    return 0;
}
