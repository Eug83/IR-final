#include <string.h>

char queryFilename[MAXFILENAME];

/*
#define MAXWORD 3000
#define MAXQUERY 100
#define MAXTERM 300
#define MAXFILENAME 1000

typedef struct query{
	char id[4];
	int termNum;
	char terms[MAXTERM][7];
} Query;

int queryNum;
Query queries[MAXQUERY];
char queryFilename[MAXFILENAME];

void printQuery(Query q){
	printf("query id: %s\n", q.id);
	for (int i = 0; i < q.termNum; i++)
		printf("%s ", q.terms[i]);
	puts("");
}

void readQueryFile(void){
	FILE* q = fopen(queryFileName, "r");
	char line[MAXWORD], nextWord[3];
	queryNum = 0;
	char split[3], period[3];
	strcpy(split, "、");
	strcpy(period, "。");
	int len, tNum, wordNum, cNum;
	while (fgets(line, MAXWORD, q) != NULL){
		if (strncmp(line, "<number>", 8) == 0){
			len = strlen(line);
			cNum = 0;
			while (cNum < 3){
				if (isdigit(line[len]))
					cNum++;
				len--;
			}
			for (int i = 0; i < 3; i++)
				queries[queryNum].id[i] = line[len + 1 + i];
			queries[queryNum].id[3] = '\0';
		}
		else if (strncmp(line, "<concepts>", 10) == 0){
			fgets(line, MAXWORD, q);
			len = strlen(line);
			tNum = 0;
			//some changes here
			for (int i = 0; i < len - 3; i += 3){
				strncpy(nextWord, line + i + 3, 3);
				if (strcmp(nextWord, split) == 0 || strcmp(nextWord, period) == 0){
					i += 3;
					continue;
				}
				strncpy(queries[queryNum].terms[tNum], line + i, 6);			
				tNum++;
			}
			queries[queryNum].termNum = tNum;
		}
		else if (strncmp(line, "</topic>", 8) == 0)
			queryNum++;
	}
	return;
}*/