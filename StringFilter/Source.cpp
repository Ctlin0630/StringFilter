#include "StringFilter.h"

using namespace std;

void main() {

	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	StringFilter SF("C:\\Project\\StringFilter\\Debug\\test.txt","C:\\Project\\StringFilter\\Debug\\filter.txt");

	//SF.ListFilter();

	/*string add_filter;
	
	while (true) {
		cout << "add filter string (\"N\" to finish):" << endl;

		cin >> add_filter;

		if (add_filter.compare("N") == 0)
			break;

		SF.AddFilterString(add_filter);
	}
	*/
	SF.StartFilter();

	/*SF.ListMatchedMap();
	cout << endl;
	SF.ListApiMatchedMap();*/
	system("pause");
}