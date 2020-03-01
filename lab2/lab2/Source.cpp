#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;
using namespace std::experimental::filesystem;


vector<path> input(const string& directory) {
	vector<path> files;
	const directory_iterator begin(directory);
	const directory_iterator end;
	copy_if(begin, end, back_inserter(files),
		[](const path& p) {
			return is_regular_file(p) && p.extension() == ".csv";
		});
	return files;
}

int main()
{
	string directory;
	getline(cin, directory);
	auto files = input(directory);
	return 0;
}