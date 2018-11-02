#pragma once
#include <iostream> 
#include <vector>
#include <string>
#include <Windows.h>
#include <fstream>
#include <sstream>

#define XSIZE 256
#define ASIZE 256

using namespace std;

struct filter_string{
	int bmGs[XSIZE];
	int bmBc[ASIZE];
	string str;
};

class BM_Alg
{
public:
	BM_Alg(char const* contentPath, char const* filterPath);
	~BM_Alg();

	string m_content;
	vector<string> m_filter;
	vector<filter_string> m_sfilter;

	void BM( string &p, string &s);
	void AddFilterString(string str);

	void preBmBc(const char *x, int m, int bmBc[]);
	void suffixes(const char *x, int m, int *suff);
	void preBmGs(const char *x, int m, int bmGs[]);

	vector<string> GetFilterString(const char* path);
	string GetContentString(const char* path);
	void SortFilter(vector<string> &v_str);
	static bool SortStringByLength(string s1, string s2);
	string UnicodeToUtf8(string);

	void createFilterTable(vector<string> &p);
	void FindAndReplaceString( string &s);
	void BM_Search(filter_string f, string &s);
	void DynamicAddFilterString( string &filter);
};

