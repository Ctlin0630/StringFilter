#include "BM_Alg.h"
#include <chrono>

using namespace std;

void main() {

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
		BM_obj.BM( f, BM_obj.m_content);
	}

	auto bm_finish = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(bm_finish - bm_start).count() << "ns" << endl;
	
	cout << BM_obj.m_content << endl;
	
	system("pause");
}