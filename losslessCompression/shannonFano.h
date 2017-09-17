#pragma once

#include "binaryTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

int bitsNeeded(int x) {
	return (x != 0) ? std::ceil(std::log(x) / std::log(2)) : 1;
}

struct node {
	char character;
	float probability;
};

int tsize;	// velicina tabele
node* table;	// tabela verovatnoca
map<char, string> dictionary;
static int nodeCompare(const void* a, const void* b);

void shannonFanno(int left, int right) {
	if (left == right)
		return;
	else if (right - left == 1) {	// interval od dva elementa
		dictionary[table[left].character] += '0';
		dictionary[table[right].character] += '1';
	}
	else {	// trazenje suma frekvencija dva intervala i sredisnjeg indeksa
		float p;
		float phalf;
		float pfull = 0;
		int splitIndex;

		for (int i = left; i <= right; ++i)
			pfull += table[i].probability;

		// trazenje indexa sredine po frekvencijama (zbirno)
		p = 0;
		splitIndex = -1; // index pozicije razdvajanja
		phalf = pfull * 0.5f;
		for (int i = left; i <= right; ++i) {
			p += table[i].probability;
			if (p <= phalf)
				dictionary[table[i].character] += '0';
			else {
				
				if (splitIndex < 0) { // prvi cvor sa >= 0.5 u slecoj iteraciji splitIndex
					splitIndex = i + 1;
					dictionary[table[i].character] += '0';
				}
				else
					dictionary[table[i].character] += '1';
			}
		}
		if (splitIndex < 0)
			splitIndex = left + 1;

		if (right - left == 2) {
			shannonFanno(left, left);
			shannonFanno(left + 1, right);
		}
		else {
			shannonFanno(left, splitIndex - 1);
			shannonFanno(splitIndex, right);
		}
	}
}

void shannonFannoCompress(char* toCompress, bool output) {
	map<char, int> frequencies;		// frekvencija pojavljivanja za svaki karakter

	int it = 0, total = 0, i;
	while (it != strlen(toCompress)) {
		char c = toCompress[it];
		frequencies[c]++;
		total++;
		++it;
	}
	tsize = (int)frequencies.size();
	// frequencies[O] : 8
	// frequencies[T] : 5
	// ...
	// tsize = 6 :: broj karaktera

	// tabela frekvencija u opadajucem redosledu :: first - karakter :: second - frekvencija pojavljivanja
	table = new node[tsize];
	float ftotal = (float)total;
	map<char, int>::iterator iter;
	for (iter = frequencies.begin(), i = 0; iter != frequencies.end(); ++iter, ++i) {
		table[i].character = iter->first;
		table[i].probability = float(iter->second / ftotal);	// zastupljenost / ftotal
	}
	// table[0]: B 0.125
	// table[1]: E 0.125
	// ...
	qsort(table, tsize, sizeof(node), nodeCompare);

	// left-index right-index
	// 0 size-1
	int left = 0;
	int right = tsize - 1;

	shannonFanno(0, tsize - 1);

	int bitsSaved = 0;
	it = 0;
	while (it != strlen(toCompress)) {
		char c = toCompress[it];
		if (dictionary[c].length() == 1)
			continue;
		else
			bitsSaved += (8 - dictionary[c].length());
		++it;
	}

	cout << endl << "Bits saved: " << bitsSaved;

	if (output) {
		cout << endl;
		int it = 0;
		while (it != strlen(toCompress)) {
			char c = toCompress[it];
			cout << dictionary[c] << " ";
			++it;
		}

		cout << endl;

		for (i = 0; i < tsize; i++)
			cout << endl << table[i].character << " :: " /*<< table[i].probability*/ << dictionary[table[i].character];
		cout << endl;
	}

	cout << endl;
}

static int nodeCompare(const void* a, const void* b) {
	node* elem1 = (node*)a;
	node* elem2 = (node*)b;
	if (elem1->probability < elem2->probability)	// 1 manje pojavljivanje :: rastuci redosled
		return 1;
	else if (elem1->probability > elem2->probability)
		return -1;
	else
		return 0;
}