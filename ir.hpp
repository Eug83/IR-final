/*
#include <stdio.h>
#include <set>
#include <stdlib.h>
#include <math.h>

using namespace std;
int feedback;
vector<int> candidates;
vector<double>::iterator vit;
int candidateNum;
Query q;
FILE* result;
char resultFilename[1000];
double weight[MAXFILENUM][MAXTERM];

void printCandidates(){
	printf("candidates: ");
	for (int i = 0; i < candidates.size(); i++)
		printf("%d ", candidates[i]);
}

void printWeight(double* w){
	printf("weight: ");
	for (int i = 0; i < q.termNum; i++)
		printf("%f ", w[i]);
	puts("");
}
void candidatesOr(map<int, int>& m){
	map<int, int>::iterator mit;
	vector<int>::iterator vit, tit;
	vector<int> temp;
	vit = candidates.begin();
	mit = m.begin();
	while (vit != candidates.end() && mit != m.end()){
		if (mit->first > *vit){
			temp.push_back(*vit);
			vit++;
		}
		else if (*vit > mit->first){
			temp.push_back(mit->first);
			mit++;
		}
		else {
			temp.push_back(*vit);
			vit++;
			mit++;
		}
	}
	for (; mit != m.end(); mit++)
		temp.push_back(mit->first);
	for (; vit != candidates.end(); vit++)
		temp.push_back(*vit);
	candidates = temp;
}

void findCandidate(Query q){
	candidates.clear();
	int termNum = q.termNum;
	Data d;
	for (int i = 0; i < termNum; i++){
		it = inverts.find(q.terms[i]);
		if (it == inverts.end())
			continue;
		d = *(it->second);
		candidatesOr(d.wordCount);
	}
	//printSet(candidates);
}

double TF(Data* d, int fileID){
	map<int, int>::iterator imit = d->wordCount.find(fileID);
	if (imit == d->wordCount.end())
		return 0;
	//yes, TF exists in wordCount(fileID -> count)
	double tf = imit->second;
	return 0.5 + 0.5 * tf / maxFreq[fileID];
	//return tf;
}

double IDF(Data* d){
	//printf("%f\n", d->fileNum * 1.0 / candidateNum);
	return log(MAXFILENUM * 1.0 / d->fileNum);	//candidateNum or total fileNum? log or log10?
}

void findWeight(int fileID){
	int termNum = q.termNum;
	map<char*, Data*>::iterator mit;
	Data* d;
	for (int i = 0; i < termNum; i++){
		mit = inverts.find(q.terms[i]);
		if (mit == inverts.end()){	//no data in inverts(vocab -> data)
			weight[fileID][i] = 0;
			continue;
		}
		d = mit->second;
		weight[fileID][i] = (TF(d, fileID) * IDF(d));
	}
	//printWeight(weight);
}
double crossSim(int fileID, double* b){
	double sum = 0;
	for (int i = 0; i < q.termNum; i++)
		sum += weight[fileID][i] * b[i];
	return sum;
}

double distSim(double* a, double* b){
	double sum = 0;
	for (int i = 0; i < q.termNum; i++)
		sum += (a[i] - b[i]) * (a[i] - b[i]);
	return sqrt(sum);
}

double cosSim(double* a, double* b){
	double up = 0, downL = 0, downR = 0;
	for (int i = 0; i < q.termNum; i++){
		up += a[i] * b[i];
		downR += a[i] * a[i];
		downL += b[i] * b[i];
	}
	return up / sqrt(downL * downR);
}

void makeQueryW(int h, double* w, set<pair<double, int> >& r){
	int termNum = q.termNum;
	if (h == 0){
		int i = 0;
		map<char*, Data*>::iterator mit;
		Data* d;
		for (int i = 0; i < termNum; i++){
			mit = inverts.find(q.terms[i]);
			if (mit == inverts.end()){	//no data in inverts(vocab -> data)
				w[i] = 0;
				continue;
			}
			d = mit->second;
			w[i] = 1 * IDF(d);
		}
	}
	else {
		//new query weight
		double a = 1.0, b = 0.8, c = 0.1;
		int top = 8;
		double dj[termNum], dk[termNum], qo[termNum];
		for (int i = 0; i < termNum; i++){
			qo[i] = w[i];
			dj[i] = 0;
			dk[i] = 0;
		}
		int count = 0;
		set<pair<double, int> >::reverse_iterator rsit;
		set<pair<double, int> >::iterator sit;
		for (rsit = r.rbegin(), sit = r.begin();
		 		rsit != r.rend(), sit != r.end(); rsit++, sit++){
			for (int i = 0; i < termNum; i++){
				dj[i] += weight[rsit->second][i];
				dk[i] += weight[sit->second][i];
			}
			count++;
			if (count == top)
				break;
		}
		for (int i = 0; i < termNum; i++)
			w[i] = a * qo[i] + b * (1.0 / top) * dj[i] + c * (1.0 / top) * dk[i];
	}
}

void printResult(set<pair<double, int> >& r){
	printf("result: \n");
	set<pair<double, int> >::reverse_iterator rsit;
	for (rsit = r.rbegin(); rsit != r.rend(); rsit++)
		printf("sim: %f, fileName: %s\n", rsit->first, files[rsit->second]);
	puts("");
}

void writeResult(set<pair<double, int> >& r){
	set<pair<double, int> >::reverse_iterator rsit;
	int count = 0;
	for (rsit = r.rbegin(); rsit != r.rend(); rsit++){
		fprintf(result, "%s %s\n", q.id, files[rsit->second]);
		count++;
		if (count == 100)
			break;
	}
}

void search(Query query){
	q = query;
	findCandidate(q);
	candidateNum = candidates.size();
	int i = 0;
	double queryW[q.termNum];//how to calc this??
	double sim;
	set<pair<double, int> > result;	//sim, fileID
	pair<double, int> p;
	for (int h = 0; h < feedback + 1; h++){
		makeQueryW(h, queryW, result);
		result.clear();
		for (int i = 0; i < candidateNum; i++){
			findWeight(candidates[i]);
			sim = crossSim(candidates[i], queryW);
			//printf("candidates %d ", candidates[i]);
			p = make_pair(sim, candidates[i]);
			result.insert(p);
		}
	}

	//printResult(result);
	writeResult(result);
	return;
}
*/