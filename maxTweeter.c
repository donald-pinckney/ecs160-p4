#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define numTop 10

struct info
{
	char *name;
	int count;
};

void invalid();
void maxTweeter(FILE *fp); 
void validate(char line[377], int tweetIndex);
char * getName(char line[377], int nameIndex);
void findColumns(FILE *fp, int *nameIndex, int *tweetIndex);
char ** getSortedNames(FILE *fp, int nameIndex, int tweetIndex, int *numLines);
int findColumnIndex(char inBuffer[], int len, const char *toFind);
char * findColumnAtIndex(char inBuffer[], int len, int index);
int strcmp_sort(const void *lhs, const void *rhs);
void info_init(struct info *topInfo);
void counter(char **names, int numLines, struct info topInfo[numTop]);
int calc_min(struct info topInfo[numTop], int *index);


void invalid()
{
	printf("Invalid Input Format\n");
	exit(1);
}

int info_sort(const void *lhs, const void *rhs)
{
	return ((const struct info *)rhs)->count - ((const struct info *)lhs)->count;
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

	struct info topInfo[numTop];

	counter(sortedNames, numLines, topInfo);

	qsort(topInfo, numTop, sizeof(struct info), info_sort);

	for(int i = 0; i < numTop; i++)
	{
		if(topInfo[i].name == NULL)
		{
			break;
		}
		printf("%s: %d\n", topInfo[i].name, topInfo[i].count);
	}
}

void validate(char line[377], int tweetIndex)
{
	char *tweet = findColumnAtIndex(line, 377, tweetIndex);
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
	return findColumnAtIndex(line, 377, nameIndex);
}


int strcmp_sort(const void *lhs, const void *rhs)
{
	return strcmp(*(const char **)lhs, *(const char **)rhs);
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
    
    *nameIndex = findColumnIndex(buffer, 377, "\"name\"");
	*tweetIndex = findColumnIndex(buffer, 377, "\"text\"");    
}

int findColumnIndex(char inBuffer[], int len, const char *toFind) 
{
	char *buffer = malloc(sizeof(char) * len);
	memcpy(buffer, inBuffer, sizeof(char) * len);

    char *token = strtok(buffer, ",");
    int i = 0;

    while (token != NULL)
    {
        if (strcmp(token, toFind) == 0)
        {
			free(buffer);
            return i;
        }
        
        token = strtok(NULL,",");
        i++;
    }
    
	free(buffer);
    return -1;
}

char * findColumnAtIndex(char inBuffer[], int len, int index) 
{
	char *buffer = malloc(sizeof(char) * len);
	memcpy(buffer, inBuffer, sizeof(char) * len);

	char *token = strtok(buffer, ",");
	if(index == 0)
	{
		char *tokenCopy = malloc(sizeof(char) * (strlen(token) + 1));
		strcpy(tokenCopy, token);
		free(buffer);
		return tokenCopy;
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
	
	char *tokenCopy = malloc(sizeof(char) * (strlen(token) + 1));
	strcpy(tokenCopy, token);
	free(buffer);

	return tokenCopy;
}

void counter(char **names, int numLines, struct info topInfo[numTop])
{
	// struct info topInfo[numTop];
	info_init(topInfo);
	int i;
	char *prev;
	char *curr;
	int currCount = 0;
	int index, currMin;

	prev = names[0];
	for(i = 0; i < numLines; i++)
	{
		curr = names[i];
		if (strcmp(prev, curr) != 0)
		{
			currMin = calc_min(topInfo, &index);
			if (currCount > currMin)
			{
				topInfo[index].name = prev;
				topInfo[index].count = currCount;
			}
			currCount = 1;
		}
		else
		{
			currCount++;
		}

		prev = curr;
	}

}

void info_init(struct info *topInfo)
{
	int j;
	
	for (j = 0; j <numTop; j++)
	{
		topInfo[j].name = NULL;
		topInfo[j].count = 0;

	}
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

int calc_min(struct info topInfo[numTop], int *index)
{
	int  i = 0;
	int currMin = topInfo[0].count;
	*index = i;

	for (i = 0; i < numTop; i++)
	{
		if (topInfo[i].count < currMin)
		{
			currMin = topInfo[i].count;
			*index = i;
		}
	}

	return currMin;
}

