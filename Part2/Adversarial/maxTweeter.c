#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

struct Tweeter {
   char name[400];
   int numTweets;
};

int compare(const void *elem1, const void *elem2) 
{
    int x = ((struct Tweeter *)elem1)->numTweets;
    int y = ((struct Tweeter *)elem2)->numTweets;
    return y - x;
}

int read_data_from_csv(const char *filepath, struct Tweeter *tweeter) {
    FILE *csv;
    const char name[7] = "\"name\"";
    const char elim[2] = ",";
    int nameCol = 0, totTweeter = 0, i, j;
    bool flag = false;
    char lineBuff[400];
    char *line;
    char *entryBuff;

    // open the file
    if (!(csv = fopen(filepath, "r"))) {
        return -1;
    }

    // check if the header is valid
    if (! fgets(lineBuff, 400, csv)) {
        return -1;
    } else {
    	line = lineBuff;
        entryBuff = strsep(&line, elim);
        while (entryBuff != NULL) {
            if (strcmp(entryBuff, name) == 0) {
                flag = true;
                break;
            }
            nameCol += 1;
            entryBuff = strsep(&line, elim);
        }
        if (! flag){
            return -1;
        }
    }

    // check each line of tweet data
    while (fgets(lineBuff, 400, csv)) {
    	line = lineBuff;
        entryBuff = strsep(&line, elim);
        i = 0;
        // find the name entry
        while (i++ < nameCol) {
            entryBuff = strsep(&line, elim);
        }
        if (entryBuff == NULL) {
            return -1;
        } else {
            // removing quotes: entryBuff contains "tweeter_name" for valid input
            if (strlen(entryBuff) > 1) {
                entryBuff[strlen(entryBuff)-1] = '\0';
                entryBuff += 1;
            }
            // update counts of tweets for the user
            flag = true;
            for (i = 0; i < totTweeter; ++i){
                if (strcmp(tweeter[i].name, entryBuff) == 0) {
                    tweeter[i].numTweets += 1;
                    flag = false;
                    break;
                }
            }
            if (flag) {
                // maximum 20000 tweeters
                if (totTweeter == 20000) {
                    return -1;
                }
                strcpy(tweeter[totTweeter].name, entryBuff);
                tweeter[totTweeter++].numTweets = 1;
            }
        }
    }
    fclose(csv);
    return totTweeter;
}

int main(int argc, char const *argv[])
{
    struct Tweeter tweeter[20010];
    int i, totTweeter;

    totTweeter = read_data_from_csv(argv[1], tweeter);
    if (totTweeter < 0) {
        printf("Invalid Input Format\n");
    } else {
        qsort(tweeter, totTweeter, sizeof(struct Tweeter), compare);

        for (i = 0; i < 10; ++i) {
            printf("%s: %d\n", tweeter[i].name, tweeter[i].numTweets);
        }
    }

    return 0;
}
