
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <map>
#include <vector>
#include <utility>

#define MAXFILENAME 1000
#define MAXWORDNUM 30000
#define MAXWORDLEN 20
#define MAXFILENUM 30000
#define MAXTERMNUM 1100000 // this should be checked

using namespace std;

char fileListFilename[MAXFILENAME];
char invFilename[MAXFILENAME];
char vocabFilename[MAXFILENAME];
// string compare for inverts
struct cmp_str{
	bool operator()(char* a, char* b){
		return strcmp(a, b) < 0;
	}
};
char words[MAXWORDNUM][MAXWORDLEN];			// wordID => word string
int maxFreq[MAXFILENUM];					// fileID => maxFreq
map<int, int> wordCounts[MAXTERMNUM];		// fileID => wordCount
char terms[MAXTERMNUM][2 * MAXWORDLEN + 1];	// termID => term string
map<char*, map<int, int>*, cmp_str> inverts;			// term string => wordCounts map



/*
#define MAXTERNUM 10

typedef struct data{
	int fileNum;
	map<int, int> wordCount; //fileID -> count
} Data;



map<char*, Data*>::iterator it;
char files[MAXFILENUM][MAXFILENAME]; //fileID -> fileName
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

void createInverts(){
	FILE* f = fopen(invFilename, "r");
	int vID1, vID2, fileNum, fileID, count;
	pair<int, int> p;
	pair<char*, map<int, int>* > item;
	int termNum = 0;
	// init maxFreq
	for (int i = 0; i < MAXFILENUM; i++)
		maxFreq[i] = 0;
	// create inverted index
	while (fscanf(f, "%d %d %d", &vID1, &vID2, &fileNum) != EOF){
		//printf("%d, %d, %d\n", vID1, vID2, fileNum);
		/*
		// No English or number
		if (vID1 < 5391 || vID1 > 12276 || vID2 < 5391 || vID2 > 12276){
			for (int i = 0; i < fileNum; i++){
				fscanf(f, "%d %d", &fileID, &count);
				//printf("fileID = %d, count = %d\n", fileID, count);
			}
			continue;
		}
		// eat data with English or number
		if (isalpha(words[vID1][0]) || isdigit(words[vID1][0])
			|| isalpha(words[vID2][0]) || isdigit(words[vID2][0])){
			for (int i = 0; i < fileNum; i++){
				fscanf(f, "%d %d", &fileID, &count);
				//printf("fileID = %d, count = %d\n", fileID, count);
			}
			continue;
		}*/

		//printf("%d, %d, %d\n", vID1, vID2, fileNum);
		// create wordCounts
		for (int i = 0; i < fileNum; i++){
			fscanf(f, "%d %d", &fileID, &count);
			//printf("fileID = %d, count = %d\n", fileID, count);
			maxFreq[fileID] = count > maxFreq[fileID] ? count : maxFreq[fileID];
			p = make_pair(fileID, count);
			wordCounts[termNum].insert(p);
		}
		// create terms
		strncpy(terms[termNum], words[vID1], strlen(words[vID1]));
		// TODO: vID2 is English?
		if (vID2 != -1)
			strncat(terms[termNum], words[vID2], strlen(words[vID2]));

		//printf("%s\n", terms[termNum]);
		item = make_pair(terms[termNum], &wordCounts[termNum]);
		inverts.insert(item);
		//printf("%s\n", item.first);
		termNum++;
	}
	/*
	map<char*, map<int, int>*, cmp_str>::iterator it;
	for (it = inverts.begin(); it != inverts.end(); it++)
		printf("%s\n", it->first);
	printf("%d\n", inverts.size());
	*/
}

/*
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