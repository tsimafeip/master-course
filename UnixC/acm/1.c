#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *replace(const char *text, const char *what, const char *with) {
    size_t whatlen = strlen(what);
    size_t withlen = strlen(with);

    char *res = malloc(10000010);
    int i = 0;
    while (*text) {
        if (strstr(text, what) == text) {
            strcpy(&res[i], with);
            i += withlen;
            text += whatlen;
        } else {
            res[i++] = *text++;
        }
    }
    res[i] = '\0';
    return res;
}

size_t SubstGlobal(const char* text, const char* what, const char* with, char* out) {
    char *result = replace(text, what, with);
    size_t len = strlen(result);
    if (out == NULL) {
        free(result);
        return len;
    } else {
        strcpy(out, result);
        free(result);
        return len;
    }
}

void fix_str(char *in) {
    size_t text_len = strlen(in);
    if (text_len > 0 && in[text_len - 1] == '\n') {
        in[text_len - 1] = '\0';
    }
}

int main() {
    char text[100002], what[102], with[102];

    fgets(text, 100002, stdin);
    fix_str(text);

    fgets(what, 102, stdin);
    fix_str(what);

    fgets(with, 102, stdin);
    fix_str(with);

    size_t len = SubstGlobal(text, what, with, NULL);
    char* out = malloc(len + 1);
    SubstGlobal(text, what, with, out);

    printf("%zu\n%s\n", len, out);
    free(out);

    return 0;
}