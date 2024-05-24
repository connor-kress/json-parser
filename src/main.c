#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "parsing.h"

int main() {
    String json_str = string_from("{\"the\": [1, 2, 3.67, null], \"hello\": {\"next\":\"world\"}}");
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
