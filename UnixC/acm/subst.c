#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXT_LEN 100002
#define MAX_WHAT_AND_WITH_LEN 102

size_t SubstGlobal(const char* text, const char* what, const char* with, char* out){
	size_t len_of_with = strlen(with);
	size_t len_of_what = strlen(what);
	size_t len_of_text = strlen(text);
	
	if (out == NULL){
		size_t counter = 0;
		while(*text){
			if (strstr(text, what) == text){
				counter++;
				text+=len_of_what;
			}
			else{
				text++;
			}
		}

		return len_of_text + counter*(len_of_with - len_of_what);
	}
	else{		
		size_t i = 0;
		while(*text){
			if (strstr(text, what) == text){
				strcpy(&out[i], with);
				i += len_of_with;
				text += len_of_what;
			}
			else{
				out[i++] = *text++;
			}
		}
		
		out[i]='\0';
		return i;
	}
}


int main() {
	char text[MAX_TEXT_LEN], what[MAX_WHAT_AND_WITH_LEN], with[MAX_WHAT_AND_WITH_LEN];

    fgets(text, MAX_TEXT_LEN, stdin);
    fgets(what, MAX_WHAT_AND_WITH_LEN, stdin);
    fgets(with, MAX_WHAT_AND_WITH_LEN, stdin);
    
    text[strlen(text) - 1] = '\0';
	what[strlen(what) - 1] = '\0';
	with[strlen(with) - 1] = '\0';
	
    size_t len = SubstGlobal(text, what, with, NULL);
    char* out = malloc(len + 1);
    SubstGlobal(text, what, with, out);

    printf("%zu\n%s\n", len, out);
    free(out);

    return 0;
}