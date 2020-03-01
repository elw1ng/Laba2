#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;
using namespace std::experimental::filesystem;

const int games_amount = 10;

struct score {
	int own;
	int other;
};

struct football_team {
	string name;
	score* matches = new score[games_amount];
	int result = 0;
};

ifstream& operator>>(ifstream& is, football_team& team) {
	getline(is >> ws, team.name, ',');
	for (auto i = 0; i < games_amount; ++i) {
		string temporary;
		if (i == games_amount - 1) getline(is >> ws, temporary);
		else getline(is >> ws, temporary, ',');
		const auto position = temporary.find(':');
		team.matches[i] = { stoi(temporary.substr(0, position)),
		stoi(temporary.substr(position + 1)) };
	}
	return is;
}

ofstream& operator<<(ofstream& out, football_team& team) {
	out << team.name << " " << team.result << endl;
	return out;
}

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

vector<football_team> parsing(vector<path>& paths) {
	vector<football_team> teams;
	for (auto& filename : paths) {
		ifstream in(filename);
		if (in.is_open()) {
			int amount;
			in >> amount;
			for (auto i = 0; i < amount; ++i) {
				football_team team;
				in >> team;
				teams.push_back(team);
			}
			in.close();
		}
		else {
			throw runtime_error("Can not open file" + filename.string());
		}
	}
	return teams;
}
int main()
{
	string directory;
	getline(cin, directory);
	auto files = input(directory);
	auto teams = parsing(files);
	return 0;
}