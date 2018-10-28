#pragma once
#include <iostream> 
#include <vector>
#include <string>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <map>
#include <list>

using namespace std;

class StringFilter
{
public:

	StringFilter(char const* contentPath, char const* filterPath);
	~StringFilter();
	void AddFilterString(string str);
	void StartFilter();
	void ListFilter();
	void ListMatchedMap();
	void ListApiMatchedMap();

private:
	string m_content;
	vector<string> m_filter;
	map<char, vector<int>> m_content_map;
	map<string, vector<int>> m_matched, m_api_matched;

	void CreateContentMapByChar();
	void FilterContentUseCharMap();
	string FilterContent();
	vector<string> GetFilterString(const char* path);
	string GetContentString(const char* path);
	void SortFilter(vector<string> &v_str);
	static bool SortStringByLength(string s1, string s2);
	void ListMap(map<string, vector<int>> map);

	//fail
	map<string, vector<int>> m_string_content_map;
	void CreateContentMap();
	void FilterContentUseMap();
	int GetUTF8WordLength(char ch);
	string UnicodeToUtf8(string);
};

