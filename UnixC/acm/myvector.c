#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER 80

struct vector {
  size_t size;
  size_t capacity;
  int* data;
};

void vector_init(struct vector* v){
	v->size = 0;
	v->capacity = 2;
	v->data = (int*) malloc(v->capacity*sizeof(int));
}

bool vector_push_back(struct vector* v, int x){
	if(v->size == v->capacity){
		if(v->capacity*2 < v->capacity)//overflow
			return false;
		
		v->capacity*=2;
		v->data = realloc(v->data, v->capacity*sizeof(int));
		
		if(!v->data) //realloc failed
			return false;
	}
	v->data[v->size++] = x;
	return true;
}

const int* vector_at(const struct vector* v, size_t index){
	if (index >= 0 && index < v->size){
		return &v->data[index];
	}
	else{
		return NULL;
	}
}

void vector_free(struct vector* v){
	v->size = 0;
	v->capacity = 0;
	free(v->data);
}

int main() {
    struct vector v;
    vector_init(&v);
	char cur_command[BUFFER];
	char temp[BUFFER];
	size_t number_to_push;
	int number_to_check;
	while(fgets(cur_command, BUFFER, stdin)){
		if (cur_command[0] == 'a'){ //at
			sscanf(cur_command, "%s %d", temp, &number_to_check);
			const int* pos = vector_at(&v, number_to_check);
			if(!pos){
				printf("%s\n", "out_of_range");
			}
			else{
				printf("%d\n", *pos);
			}
		}
		else if (cur_command[0] == 'p'){ //push back
			sscanf(cur_command, "%s %zu", temp, &number_to_push);
            vector_push_back(&v, number_to_push);
		}
		else{
			break;
		}
	}
	
    vector_free(&v);
    return 0;
}