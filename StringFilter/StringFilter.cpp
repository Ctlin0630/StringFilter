#include "StringFilter.h"
#include <codecvt>
#include <locale>
#include <algorithm>

using namespace std;

StringFilter::StringFilter(char const* contentPath, char const* filterPath)
{

	m_filter = GetFilterString(filterPath);

	m_content = GetContentString(contentPath);

	int s = ::GetTickCount();
	CreateContentMapByChar();
	int e = ::GetTickCount();

	cout << "******create map time :" << e - s << endl;

}


StringFilter::~StringFilter()
{
}

void StringFilter::StartFilter(){
	int s = ::GetTickCount();
	FilterContentUseCharMap();
	int e = ::GetTickCount();

	cout << "******search time :" << e - s << endl;

	s = ::GetTickCount();
	FilterContent();
	e = ::GetTickCount();

	cout << "******api search time :" << e - s << endl;
}

vector<string> StringFilter::GetFilterString(const char* path) {

	std::vector<std::string> filter;
	ifstream infile; 
	string strLine = ""; 
	infile.open(path);
	if (infile)
	{
		while (!infile.eof()) {
			getline(infile, strLine);
			filter.push_back( strLine );
		}
	}
	infile.close();
	
	return filter;
	
}

string StringFilter::GetContentString(const char* path) {

	std::stringstream content;
	ifstream infile; 
	infile.open(path);
	if (infile)
	{
		content << infile.rdbuf();
	}
	infile.close();

	return content.str();

}

void StringFilter::AddFilterString(string str) {

	m_filter.push_back(UnicodeToUtf8(str));
}

string StringFilter::FilterContent() {

	for ( vector<string>::iterator it = m_filter.begin(); it != m_filter.end(); it++) {

		size_t pos = m_content.find( *it );

		while (pos != std::string::npos)
		{
			InsertHitMap(*it, pos, m_api_matched);

			pos = m_content.find(*it, pos + it->size());
		}
	}
	return m_content;
}

int StringFilter::GetUTF8WordLength(char ch) {

	int cplen = 1;
	if ((ch & 0xf8) == 0xf0)
		cplen = 4;
	else if ((ch & 0xf0) == 0xe0)
		cplen = 3;
	else if ((ch & 0xe0) == 0xc0)
		cplen = 2;

	return cplen;
}

void StringFilter::CreateContentMap() {

	for (size_t i = 0; i < m_content.length();){
		int cplen = GetUTF8WordLength(m_content[i] );

		if ((i + cplen) > m_content.length())
			cplen = 1;

		m_string_content_map[m_content.substr(i, cplen)].push_back(i);

		i += cplen;
	}
	return;
}

void StringFilter::CreateContentMapByChar() {

	for (size_t i = 0; i < m_content.length(); i++){
		m_content_map[m_content[i]].push_back(i);
	}
	return;
}


void StringFilter::ListMatchedMap() {
	ListMap(m_matched);
}

void StringFilter::ListApiMatchedMap() {
	ListMap(m_api_matched);
}

void StringFilter::ListMap(map<string, vector<int>> map) {
	for (auto& t : map) {
		cout << "key:" << t.first << endl << "value:";
		for (auto& i = t.second.begin(); i != t.second.end(); ++i)
			cout << *i << ",";
		cout << endl;
	}
}

void StringFilter::SortFilter(vector<string> &v_str) {

	std::sort(v_str.begin(), v_str.end(), SortStringByLength);
}

bool StringFilter::SortStringByLength(string s1, string s2) {

	int s1Len = ::MultiByteToWideChar(CP_UTF8, NULL, s1.c_str(), strlen(s1.c_str()), NULL, 0);
	int s2Len = ::MultiByteToWideChar(CP_UTF8, NULL, s2.c_str(), strlen(s2.c_str()), NULL, 0);
	return s1Len > s2Len;
}

void StringFilter::FilterContentUseMap() {

	for (auto& f : m_filter) {

		int cplen = GetUTF8WordLength(f[0]);

		if ((unsigned)( cplen) > f.length())
			cplen = 1;

		f.substr(0, cplen);

		auto& it = m_string_content_map.find( f.substr(0, cplen) );

		if (it == m_string_content_map.end())
			continue;

		for (auto& i = it->second.begin(); i != it->second.end(); ++i) {

			if (f.compare(m_content.substr(*i, f.length())) != 0)
				continue;
			
			InsertHitMap(f, *i, m_matched);
		}
	}
}

void StringFilter::FilterContentUseCharMap() {

	for (auto& f : m_filter) {

		auto& it = m_content_map.find(f.at(0));

		if (it == m_content_map.end())
			continue;

		for (auto& i = it->second.begin(); i != it->second.end(); ++i) {

			if (f.compare(m_content.substr(*i, f.length())) != 0)
				continue;

			InsertHitMap(f, *i, m_matched);
		}
	}
}

void StringFilter::InsertHitMap(string f, int i, map<string, vector<int>> &hit_map) {

	auto& hit_it = hit_map.find(f);

	if (hit_it == hit_map.end()) {
		vector<int> temp;
		temp.push_back(i);
		hit_map.insert(pair<string, vector<int>>(f, temp));
	}
	else {
		hit_map[f].push_back(i);
	}
}

void StringFilter::ListFilter() {

	for (auto &it: m_filter) {
		cout << it << endl;
	}
}

string StringFilter::UnicodeToUtf8(string str) {

	int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(),
		str.length(), nullptr, 0);

	std::wstring utf16_str(size, '\0');

	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(),
		str.length(), &utf16_str[0], size);

	int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), nullptr, 0,
		nullptr, nullptr);

	std::string utf8_str(utf8_size, '\0');

	WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), &utf8_str[0], utf8_size,
		nullptr, nullptr);

	return utf8_str;
}