#include "BM_Alg.h"
#include <algorithm>

BM_Alg::BM_Alg(char const* contentPath, char const* filterPath)
{

	m_filter = GetFilterString(filterPath);

	m_content = GetContentString(contentPath);

}



BM_Alg::~BM_Alg()
{
}

vector<string> BM_Alg::GetFilterString(const char* path) {

	std::vector<std::string> filter;
	ifstream infile;
	string strLine = "";
	infile.open(path);
	if (infile)
	{
		while (!infile.eof()) {
			getline(infile, strLine);
			filter.push_back(strLine);
		}
	}
	infile.close();

	return filter;

}

string BM_Alg::GetContentString(const char* path) {

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

void BM_Alg::AddFilterString(string str) {

	m_filter.push_back(UnicodeToUtf8(str));
}

string BM_Alg::UnicodeToUtf8(string str) {

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

void BM_Alg::SortFilter(vector<string> &v_str) {

	std::sort(v_str.begin(), v_str.end(), SortStringByLength);
}

bool BM_Alg::SortStringByLength(string s1, string s2) {

	int s1Len = ::MultiByteToWideChar(CP_UTF8, NULL, s1.c_str(), strlen(s1.c_str()), NULL, 0);
	int s2Len = ::MultiByteToWideChar(CP_UTF8, NULL, s2.c_str(), strlen(s2.c_str()), NULL, 0);
	return s1Len > s2Len;
}

void BM_Alg::preBmBc(const char *x, int m, int bmBc[]) {
	int i;

	for (i = 0; i < ASIZE; ++i)
		bmBc[i] = m;
	for (i = 0; i < m - 1; ++i)
		bmBc[(unsigned char)x[i]] = m - i - 1;
}


void BM_Alg::suffixes(const char *x, int m, int *suff) {
	int f, g, i;

	suff[m - 1] = m;
	g = m - 1;
	for (i = m - 2; i >= 0; --i) {
		if (i > g && suff[i + m - 1 - f] < i - g)
			suff[i] = suff[i + m - 1 - f];
		else {
			if (i < g)
				g = i;
			f = i;
			while (g >= 0 && x[g] == x[g + m - 1 - f])
				--g;
			suff[i] = f - g;
		}
	}
}

void BM_Alg::preBmGs(const char *x, int m, int bmGs[]) {
	int i, j, suff[XSIZE];

	suffixes(x, m, suff);

	for (i = 0; i < m; ++i)
		bmGs[i] = m;
	j = 0;
	for (i = m - 1; i >= 0; --i)
		if (suff[i] == i + 1)
			for (; j < m - 1 - i; ++j)
				if (bmGs[j] == m)
					bmGs[j] = m - 1 - i;
	for (i = 0; i <= m - 2; ++i)
		bmGs[m - 1 - suff[i]] = m - 1 - i;
}



void BM_Alg::BM( string &p, string &s) {
	int i, j, bmGs[XSIZE], bmBc[ASIZE];

	int p_len = p.length();
	int s_len = s.length();
	/* Preprocessing */
	preBmGs(p.c_str(), p_len, bmGs);
	preBmBc(p.c_str(), p_len, bmBc);

	/* Searching */
	j = 0;
	while (j <= s_len - p_len) {
		for (i = p_len - 1; i >= 0 && p[i] == s[i + j]; --i);
		if (i < 0) {
			//cout << " pattern occurs at shift = " << j << " " << s.substr(j, p_len).c_str() << endl;;
			s.replace(j, p_len, "");

			s_len -= p_len;

			j += bmGs[0];
		}
		else {
			j += max(bmGs[i], bmBc[(unsigned char)s[i + j]] - p_len + 1 + i);
		}

	}
}

void BM_Alg::createFilterTable(vector<string> &filters) {

	for (int i = 0; i < filters.size(); ++i) {

		filter_string f;

		preBmGs(filters[i].c_str(), filters[i].length(), f.bmGs);

		preBmBc(filters[i].c_str(), filters[i].length(), f.bmBc);

		f.str = filters[i];

		m_sfilter.push_back( f );
	}
}

void BM_Alg::FindAndReplaceString( string &s ) {
	for (auto f : m_sfilter) 
		BM_Search( f, s );	
}

void BM_Alg::BM_Search(filter_string f, string &s) {

	int i, j;

	int p_len = f.str.length();
	int s_len = s.length();

	j = 0;
	while (j <= s_len - p_len) {
		for (i = p_len - 1; i >= 0 && f.str[i] == s[i + j]; --i);
		if (i < 0) {
			//cout << " pattern occurs at shift = " << j << " " << s.substr(j, p_len).c_str() << endl;;
			s.replace(j, p_len, "");

			s_len -= p_len;

			j += f.bmGs[0];
		}
		else {
			j += max(f.bmGs[i], f.bmBc[(unsigned char)s[i + j]] - p_len + 1 + i);
		}

	}
}