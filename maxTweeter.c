#include <stdio.h>

void maxTweeter(char *filename) {

}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Invalid arguments\n");
		exit(1);
	}
	char *filename = argv[1];

	maxTweeter(filename); 
}

