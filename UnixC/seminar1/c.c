#include <unistd.h> 

int main() {
	int N = 1000000;
	for (int i = 0; i < N; i++){
		getpid();
	}
	return 0;
}
