#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int bitsNeededL(int x) {
	return (x != 0) ? std::ceil(std::log(x) / std::log(2)) : 1;
}

string searchMap(map<string, int> dictionary, int mappedValue) {
	for (map<string, int>::iterator it = dictionary.begin(); it != dictionary.end(); ++it)
		if (it->second == mappedValue)
			return it->first;
}

void printMap(map<string, int> dictionary) {
	for (map<string, int>::iterator it = dictionary.begin(); it != dictionary.end(); ++it)
		if(it->second > 255)
			cout << endl << it->first << " :: " << it->second;
}

void lzwCompress(char* toCompress, bool output) {
	int dictSize = 256;
	map<string, int> dictionary; // asocijativna mapa ["key"] -> mapped
	for (int i = 0; i < 256; i++)
		dictionary[string(1, i)] = i;

	// dictionary["0"] : 48
	// dictioanry["A"] : 65
	// dictionary["a"] : 97
	// ...

	// dictionary["<>"] : 256
	// ...

	int it = 0, cmp = 0;
	string w;
	vector<int> compressed;
	while (it != strlen(toCompress)) {
		char c = toCompress[it];
		string wc = w + c;
		if (dictionary.count(wc)) // wc u recniku..
			w = wc;				  // w postaje duzi podniz
		else {
			// Dodavanje wc u recnik..
			compressed.push_back(dictionary[w]); // na kraj output (compressed) vektora :: w je podniz/karakter iz prethodne iteracije koji je u recniku 0..255 ili 255+
			dictionary[wc] = dictSize++;		 // dictionary[wc] = 255++
			w = string(1, c);
		}
		++it;
	}
	// ..TO vec u recniku
	// ..TOB nije u recniku -- dodaje se na kraj recnika
	// TO je najduzi podniz TOB koji je u recniku -- dodaje se na output <256> :: w=B za sledecu iteraciju

	if (!w.empty())
		compressed.push_back(dictionary[w]);

	int bitsSaved = 0;
	for (vector<int>::const_iterator i = compressed.begin(); i != compressed.end(); ++i)
		if (*i < 255)
			continue;
		else
			bitsSaved += searchMap(dictionary, *i).length() * 8 - bitsNeeded(*i);
	cout << endl << "Bits saved: " << bitsSaved;

	if (output) {
		cout << endl;
		for (vector<int>::const_iterator i = compressed.begin(); i != compressed.end(); ++i)
			if (*i < 255)
				cout << searchMap(dictionary, *i) << ' ';
			else
				cout << '<' << *i  << '>' << ' ';
		cout << endl;
		printMap(dictionary);
		cout << endl;
	}
}