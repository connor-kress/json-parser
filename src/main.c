#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "parsing.h"

int main() {
    Json list = new_list();
    for (int i = 0; i < 5; i++) {
        push_json(list.data, int_from(i));
    }
    Json list_sub = new_list();
    for (int i = 0; i < 3; i++) {
        push_json(list_sub.data, int_from(i));
    }
    push_json(list.data, list_sub);
    for (int i = 5; i < 10; i++) {
        push_json(list.data, int_from(i));
    }

    Json obj = new_object();
    add_attr(obj.data, string_from("first"), int_from(4));
    add_attr(obj.data, string_from("second"), json_string_from_cstr("hello?"));
    add_attr(obj.data, string_from("listy"), list);
    add_attr(obj.data, string_from("is_dar"), bool_from(true));
    add_attr(obj.data, string_from("is_ok"), bool_from(false));
    add_attr(obj.data, string_from("is_hot"), new_null());
    print_json(&obj);
    putchar('\n');

    free_json(obj);

    String num_str = string_from("1234");
    printf("Length: %lu\n", num_str.len);
    printf("Val: %d\n", atoi(num_str.data));
    return 0;
}
