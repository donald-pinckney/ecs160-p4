#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void invalid();
void maxTweeter(FILE *fp); 
void validate(char line[377], int tweetIndex);
char * getName(char line[377], int nameIndex);
void findColumns(FILE *fp, int *nameIndex, int *tweetIndex);
char ** getSortedNames(FILE *fp, int nameIndex, int tweetIndex, int *numLines);
int findColumnIndex(char buffer[], const char *toFind);
char * findColumnAtIndex(char buffer[], int index); 
int strcmp_sort(const void *lhs, const void *rhs);


void invalid()
{
	printf("Invalid Input Format\n");
	exit(1);
}

void maxTweeter(FILE *fp) 
{
    int nameIndex, tweetIndex;
	findColumns(fp, &nameIndex, &tweetIndex);
	if(nameIndex == -1 || tweetIndex == -1) 
	{
		invalid();
	}


	int numLines;
	char **sortedNames = getSortedNames(fp, nameIndex, tweetIndex, &numLines);

}

void validate(char line[377], int tweetIndex)
{
	char *tweet = findColumnAtIndex(line, tweetIndex);
	char c;
	while((c = *tweet) != '\0')
	{
		if(c == ',')
		{
			invalid();
		}
		tweet += 1;
	}
}

char * getName(char line[377], int nameIndex)
{
	return findColumnAtIndex(line, nameIndex);
}


int strcmp_sort(const void *lhs, const void *rhs)
{
	return strcmp((const char *)lhs, (const char *)rhs);
}

char ** getSortedNames(FILE *fp, int nameIndex, int tweetIndex, int *numLines)
{
	char **nameRows = malloc(20000 * sizeof(char *));
	memset(nameRows, 0, sizeof(char *) * 20000);

	char line[377];
	int i = 0;

	while(fgets(line, 377, fp) != NULL)
	{
		validate(line, tweetIndex);

		char *name = getName(line, nameIndex);

		nameRows[i] = name;
		i++;
	}

	*numLines = i;

	char **actualNameRows = malloc(sizeof(char *) * i);
	memcpy(actualNameRows, nameRows, sizeof(char *) * i);
	free(nameRows);

	qsort(actualNameRows, i, sizeof(char *), strcmp_sort);

	return actualNameRows;
}



void findColumns(FILE *fp, int *nameIndex, int *tweetIndex)
{
    char buffer[377], *token;
    
    if (fgets(buffer , 377, fp) == NULL)
    {
        invalid();
    }
    
    *nameIndex = findColumnIndex(buffer, "\"name\"");
	*tweetIndex = findColumnIndex(buffer, "\"text\"");    
}

int findColumnIndex(char buffer[], const char *toFind) 
{
    char *token = strtok(buffer, ",");
    int i = 0;

    while (token != NULL)
    {
        if (strcmp(token, toFind) == 0)
        {
            return i;
        }
        
        token = strtok(NULL,",");
        i++;
    }
    
    return -1;
}

char * findColumnAtIndex(char buffer[], int index) 
{
	char *token = strtok(buffer, ",");
	if(index == 0)
	{
		return token;
	}

	int i;
	for(i = 1; i < index + 1; i++)
	{
		token = strtok(NULL, ",");
		if(token == NULL)
		{
			invalid();
		}
	}
	
	return token;
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



