#include<omp.h>
#include<cstdio>
#include<cstdlib>
#include<cctype>
#include<cstring>
#include<dirent.h>
#include<set>
#include<map>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

DIR *dptr;
struct dirent *direntptr;
ifstream fin;
ofstream fout;
vector<string> input;

set<string> keyword;
string word;
map<string, int> dict;

int main(int argc, char* argv[])
{
	fin.open("keyword.txt");
	while(fin >> word) {
		keyword.insert(word);
		dict[word] = 0;
	}
	fin.close();


	dptr = opendir("input");
	if(dptr == NULL) {
		puts("The input directory does not exist.");
		return 1;
	}

	while((direntptr = readdir(dptr)) != NULL) {
		if(direntptr->d_name[0] == '.') continue;
		char prefix[100]; strcpy(prefix, "input/");
		string filename(strcat(prefix, direntptr->d_name));
		input.push_back(filename);
	}

#pragma omp parallel for private(word) num_threads(atoi(argv[1]))
	for(int i = 0; i < input.size(); i++) {
		ifstream ffin(input[i].c_str());

		string s;
		while(ffin >> s) {
			for(int j = 0; j < s.length(); j++)
				if(isalpha(s[j])) s[j] = tolower(s[j]); else s[j] = ' ';

			stringstream ss(s);
			while(ss >> word) {
				if(!keyword.count(word)) continue;
#pragma omp cirtical
				dict[word]++;
			}
		}

		fin.close();
	}


	fin.open("keyword.txt");
	fout.open("output.txt");
	while(fin >> word) {
		fout << word << ' ' << dict[word] << endl;
	}
	fin.close();
	fout.close();

	return 0;
}
