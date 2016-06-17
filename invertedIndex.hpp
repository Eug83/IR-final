
#include <stdio.h>
#include <ctype.h>
#include <map>
#include <vector>
#include <utility>

#define MAXFILENAME 1000
#define MAXWORDNUM 30000
#define MAXWORDLEN 20

char fileListFilename[MAXFILENAME];
char invFilename[MAXFILENAME];
char vocabFilename[MAXFILENAME];

char words[MAXWORDNUM][MAXWORDLEN];
/*
#define MAXTERNUM 10
#define MAXTERMNUM 1100000 //1065981?
#define MAXFILENUM 46973
using namespace std;

typedef struct data{
	int fileNum;
	map<int, int> wordCount; //fileID -> count
} Data;

struct cmp_str{
	bool operator()(char* a, char* b){
		return strcmp(a, b) < 0;
	}
};

Data datum[MAXTERMNUM];
char terms[MAXTERMNUM][7];
map<char*, Data*, cmp_str> inverts;
map<char*, Data*>::iterator it;
char files[MAXFILENUM][MAXFILENAME]; //fileID -> fileName
int maxFreq[MAXFILENUM];	//fileID -> maxFreq
int fileNum;
char modelDir[1000];

void printData(Data d){
	int fileNum = d.fileNum;
	map<int, int> wordCount = d.wordCount;
	printf("fileNum = %d\n", fileNum);
	map<int, int>::iterator mit;
	for (mit = wordCount.begin(); mit != wordCount.end(); mit++)
		printf("fileID: %d, count: %d\n", mit->first, mit->second);
	return;
}
*/
void readVocab(){
	FILE* f = fopen(vocabFilename, "r");
	int wordNum = 0;
	while (fscanf(f, "%s", words[wordNum]) != EOF)
		wordNum++;
}
/*
void createInverts(){
	char invertedFile[1000];
	strncpy(invertedFile, modelDir, 1000);
	strncat(invertedFile, "/inverted-file", 30);
	FILE* f = fopen(invertedFile, "r");
	int vID1, vID2, fileNum, fileID, count;
	pair<int, int> p;
	pair<char*, Data*> item;
	int termNum = 0;

	for (int i = 0; i < MAXFILENUM; i++)
		maxFreq[i] = 0;
	
	while (fscanf(f, "%d %d %d", &vID1, &vID2, &fileNum) != EOF){
		//printf("%d, %d, %d\n", vID1, vID2, fileNum);
		//No English and number
		if (vID1 < 5391 || vID1 > 12276 || vID2 < 5391 || vID2 > 12276){
			for (int i = 0; i < fileNum; i++){
				fscanf(f, "%d %d", &fileID, &count);
				//printf("fileID = %d, count = %d\n", fileID, count);
			}
			continue;
		}
		if (isalpha(words[vID1][0]) || isdigit(words[vID1][0])
			|| isalpha(words[vID2][0]) || isdigit(words[vID2][0])){
			for (int i = 0; i < fileNum; i++){
				fscanf(f, "%d %d", &fileID, &count);
				//printf("fileID = %d, count = %d\n", fileID, count);
			}
			continue;
		}

		//printf("%d, %d, %d\n", vID1, vID2, fileNum);
		datum[termNum].fileNum = fileNum;
		for (int i = 0; i < fileNum; i++){
			fscanf(f, "%d %d", &fileID, &count);
			//printf("fileID = %d, count = %d\n", fileID, count);
			maxFreq[fileID] = count > maxFreq[fileID] ? count : maxFreq[fileID];
			p = make_pair(fileID, count);
			datum[termNum].wordCount.insert(p);
		}
		
		strncpy(terms[termNum], words[vID1], 3);
		if (vID2 != -1)
			strncat(terms[termNum], words[vID2], 3);
		terms[termNum][6] = '\0';

		item = make_pair(terms[termNum], &datum[termNum]);
		inverts.insert(item);
		//printf("%s\n", item.first);
		termNum++;
	}
	//for (it = inverts.begin(); it != inverts.end(); it++)
	//	printf("%s\n", it->first);
}

void createFileName(){
	char fileList[1000];
	strncpy(fileList, modelDir, 1000);
	strncat(fileList, "/file-list", 30);
	FILE* f = fopen(fileList, "r");
	fileNum = 0;
	while (fscanf(f, "%s", files[fileNum]) != EOF){
		for (int i = 0; i < 15; i++)
			files[fileNum][i] = tolower(files[fileNum][i + 16]);
		files[fileNum][15] = '\0';
		fileNum++;
	}
}
*/
void createInvertedIndex(){
	readVocab();
	createInverts();
	//createFileName();
}