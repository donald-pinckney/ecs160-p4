
/*
* ECS 160 Winter 2018 Homework 4
* Authors : Donald Pinckney, Megha Jain
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define numTop 10 //The top n tweeters to get.

/*
* This struct holds the information required to keep track of current top 
* ten tweeters -- name of the tweeter and thier tweet count. 
*/


struct info
{
	char *name;
	int count;
};

/*
* Function prototypes, because yay modularity!
*/
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



/*
* Error handler function for when we encounter invalid inputs. 
*/
void invalid()
{
	printf("Invalid Input Format\n");
	exit(1);
}

/*
* Sort function passed as parameter to qsort in order to sort the top ten in descending order.
*/
int info_sort(const void *lhs, const void *rhs)
{
	return ((const struct info *)rhs)->count - ((const struct info *)lhs)->count;
}

/*
* This functions is the controller called after main that controls the execution flow of the program.
* After processing, given no invalid input cases, it prints the top ten tweeters. 
*/
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

/*
* This function gets the name of the tweeter in the current line.
*/
char * getName(char line[377], int nameIndex)
{
	return findColumnAtIndex(line, 377, nameIndex);
}


/*
* The sort function a parameter to qsort for sorting names alphabetically.
*/
int strcmp_sort(const void *lhs, const void *rhs)
{
	return strcmp(*(const char **)lhs, *(const char **)rhs);
}

/*
* This function returns an alphabetically sorted array of the names of all the tweeters.
* Note that this function keeps the redundant entries for each tweeter because that is
* used later to compute tweet count for every tweeter.  
*/
char ** getSortedNames(FILE *fp, int nameIndex, int tweetIndex, int *numLines)
{
	char **nameRows = malloc(20000 * sizeof(char *));
	memset(nameRows, 0, sizeof(char *) * 20000);

	char line[378];
	int i = 0;

	while(fgets(line, 378, fp) != NULL)
	{
		if(line[0] == '\n')
		{
			invalid();
		}

		if(strlen(line) == 377)
		{
			invalid();
		}

		line[strlen(line) - 1] = '\0';

		if(strlen(line) == 0)
		{
			invalid();
		}

		validate(line, tweetIndex);

		char *name = getName(line, nameIndex);

		nameRows[i] = name;
		i++;

		if(i == 20000)
		{
			invalid();
		}
	}

	*numLines = i;

	char **actualNameRows = malloc(sizeof(char *) * i);
	memcpy(actualNameRows, nameRows, sizeof(char *) * i);
	free(nameRows);

	qsort(actualNameRows, i, sizeof(char *), strcmp_sort);



	return actualNameRows;
}


/*
* This function finds the indices for the name and text columns respectively. 
* If they aren't found, then nameIndex and tweetIndex will be set to -1 which 
* is then handled by the invocating method. 
*/
void findColumns(FILE *fp, int *nameIndex, int *tweetIndex)
{
    char buffer[378], *token;
    
    if (fgets(buffer , 377, fp) == NULL)
    {
        invalid();
    }

	if(strlen(buffer) == 377)
	{
		invalid();
	}

	buffer[strlen(buffer) - 1] = '\0';

	if(strlen(buffer) == 0)
	{
		invalid();
	}
    
    *nameIndex = findColumnIndex(buffer, 377, "\"name\"");
	*tweetIndex = findColumnIndex(buffer, 377, "\"text\"");    
}



/*
* This functions find the index for the column of a given name. 
*/
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


/*
* This functions extracts the field at a given column index and returns it. 
*/
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


/*
* This function is the central function that computes that top ten tweeters. 
* It does so by keeping track of the current top ten ones whilst traversing the 
* loop of sorted (redundant) names and checking on computing count for a 
* new tweeter is greater than the the current minimum count. If yes, it swaps
* the new tweeter and its count with the old minimum.
*/
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

	currMin = calc_min(topInfo, &index);
	if (currCount > currMin)
	{
		topInfo[index].name = prev;
		topInfo[index].count = currCount;
	}
}


/*
* Initialises the struct array that holds information about the top ten tweeters. 
*/
void info_init(struct info *topInfo)
{
	int j;
	
	for (j = 0; j <numTop; j++)
	{
		topInfo[j].name = NULL;
		topInfo[j].count = 0;

	}
}



/*
* Main function, opens file of name passed as command line argument and calls the rest of 
* the necessary functions. 
*/
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
	if (file == NULL)
	{
		perror("Could not find file");
		exit(1);
	}
    
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

