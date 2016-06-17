#include "invertedIndex.hpp"
#include "queryReader.hpp"
#include "ir.hpp"

void parseArg(int argc, char** argv){
	for (int i = 0; i < argc; i++)
		if (strcmp(argv[i], "-f") == 0){
			strncpy(fileListFilename, argv[i + 1], MAXFILENAME);
			i++;
		}
		else if (strcmp(argv[i], "-v") == 0){
			strncpy(vocabFilename, argv[i + 1], MAXFILENAME);
			i++;
		}
		else if (strcmp(argv[i], "-i") == 0){
			strncpy(invFilename, argv[i + 1], MAXFILENAME);
			i++;
		}
		else if (strcmp(argv[i], "-q") == 0){
			strncpy(queryFilename, argv[i + 1], MAXFILENAME);
			i++;
		}
}

int main(int argc, char** argv){
	parseArg(argc, argv);
	createInvertedIndex();

	map<char*, map<int, int>*, cmp_str>::iterator it;
	for (it = inverts.begin(); it != inverts.end(); it++)
		printf("%s\n", it->first);

	readQueryFile();

	for (int i = 0; i < queryTermNum; i++)
		printf("%s\n", queryTerms[i]);

	search();
	return 0;
}
