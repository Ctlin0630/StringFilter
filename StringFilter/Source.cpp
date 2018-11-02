#include "BM_Alg.h"
#include <chrono>
#include <time.h>

using namespace std;

/*void gen_random(string &s) {

	static const char alphanum[] =
		"012gen_random3456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	int len = rand() % 50 + 50;
	char c[100];
	for (int i = 0; i < len; ++i) {
		c[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	c[len] = 0;

	s = c;
}*/

void main() {
	/*srand(static_cast<int>(time(0)));
	ofstream myfile("filter.txt");
	for (long i = 0; i < 200000; i++) {

		string s;
		gen_random(s);
		myfile << s + "\n";
	}
	myfile.close();
	return ;*/

	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	BM_Alg BM_obj("C:\\Project\\StringFilter\\Debug\\test.txt","C:\\Project\\StringFilter\\Debug\\filter.txt");
	
	string add_filter;

	while (true) {
		cout << "add filter string (\"N\" to finish):" << endl;

		cin >> add_filter;

		if (add_filter.compare("N") == 0)
			break;

		BM_obj.AddFilterString(add_filter);
	}

	BM_obj.SortFilter(BM_obj.m_filter);

	auto bm_start = std::chrono::high_resolution_clock::now();

	for (auto& f : BM_obj.m_filter) {
		if (f.length() == 0)
			continue;

		BM_obj.BM( f, BM_obj.m_content);
	}

	auto bm_finish = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(bm_finish - bm_start).count() << "ns" << endl;
	
	cout << BM_obj.m_content << endl;

	BM_obj.createFilterTable( BM_obj.m_filter );
	
	BM_obj.FindAndReplaceString( BM_obj.m_content );
	system("pause");
}