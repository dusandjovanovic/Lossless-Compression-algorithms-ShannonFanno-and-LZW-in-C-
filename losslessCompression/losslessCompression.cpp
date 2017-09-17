// losslessCompression.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "shannonFano.h"
#include "LZW.h"

#include <cstdio>
#include <ctime>

int main()
{
	char *string = "TOBEORNOTTOBEORTOBEORNOT";
	lzwCompress(string, true);
	shannonFannoCompress(string, true);

	char fileName[20] = "100000.txt";
	std::clock_t start;
	double duration;

	while (strcmp(fileName, "end") != 0) {
		cout << endl << "Input filename" << endl;
		cin >> fileName;
		bool output = false;
		FILE *f = fopen(fileName, "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		char *string = (char *)malloc(fsize + 1);
		fread(string, fsize, 1, f);
		fclose(f);
		string[fsize] = 0;

		if (strcmp(fileName, "100.txt") == 0) {
			output = true;
		}

		std::fflush(stdout);
		start = std::clock();

		lzwCompress(string, output);

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << endl << "LZW time: " << duration << '\n';

		std::fflush(stdout);
		start = std::clock();

		shannonFannoCompress(string, output);

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "Shannon-fano time: " << duration << '\n';
	}

	system("pause");
    return 0;
}