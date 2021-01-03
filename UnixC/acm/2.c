#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct vector {
    int * array;
    size_t len;
    size_t max;
};

void vector_init(struct vector* v) {
    v->max = 2;
    v->array = (int*) malloc(v->max * sizeof(int));
    v->len = 0;
}

bool vector_push_back(struct vector* v, int x) {
    if (v->len >= v->max) {
        size_t new_max = v->max * 2;
        if (new_max < v->max) {
            //overflow
            return false;
        }
        int * new_arr = (int*) malloc(new_max * sizeof(int));
        if (new_arr == NULL) {
            //malloc, why?
            return false;
        }
        for (size_t i = 0; i < v->len; i++) {
            new_arr[i] = v->array[i];
        }
        //memcpy(new_arr, v->array, v->max);
        free(v->array);
        v->max = new_max;
        v->array = new_arr;
    }
    v->array[v->len] = x;
    v->len++;
    return true;
}

const int* vector_at(const struct vector* v, size_t index) {
    if (index >= v->len) {
        return NULL;
    } else {
        return &v->array[index];
    }
}

void vector_free(struct vector* v) {
    free(v->array);
    v->len = 0;
    v->max = 1;
}

int main() {
    struct vector v;
    vector_init(&v);
    char command[80];
    char thrash[80];
    while(fgets(command, 80, stdin)) {
        if (command[0] == 'p') {
            int x;
            sscanf(command, "%s %d", thrash, &x);
            vector_push_back(&v, x);
        } else if (command[0] == 'a') {
            size_t t;
            sscanf(command, "%s %zu", thrash, &t);
            const int * r = vector_at(&v, t);
            if (r == NULL) {
                printf("out_of_range\n");
            } else {
                printf("%d\n", *r);
            }
        } else {
            break;
        }
    //   for (int i = 0; i < v.len; i++) {
    //       printf("%d ", v.array[i]);
    //   }
    //   printf(". %zu %zu\n", v.len, v.max);
    }
    vector_free(&v);
    return 0;
}