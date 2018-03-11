#include <stdio.h>

void maxTweeter(FILE *fp) 
{
    
}

int main(int argc, char *argv[]) 
{
	if(argc != 2) 
    {
		printf("Invalid arguments\n");
		exit(1);
	}
	char *filename = argv[1];

    FILE * file;
    file = fopen(filename, "r");
    
	maxTweeter(file); 
}

int findColumn(FILE *fp)
{
    
    char buffer[377], *token;
    int i;
    i = 0;
    
    if (fgets(buffer , 377, fp) == NULL)
    {
        return -1;
    }
    
    
    token = strtok(buffer, ",");
    
    while (token != NULL)
    {
        
        if (strcmp(token, "\"name\"") == 0)
        {
            return i;
        }
        
        token = strtok(NULL,",");
        i++;
    }
    
    return -1;
    
}








