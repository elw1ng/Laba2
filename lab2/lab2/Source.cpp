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

void calculate_result(football_team& team) {
	for (auto i = 0; i < games_amount; ++i) {
		team.result += team.matches[i].own > team.matches[i].other ? 3 :
			team.matches[i].own == team.matches[i].other ? 1 : 0;
	}
}

void calculate_result_to_all(vector<football_team>& teams) {
	for (auto& team : teams) {
		calculate_result(team);
	}
}

void sort_teams(vector<football_team>& teams) {
	for (auto j = 1; j < teams.size(); ++j) {
		const auto key = teams[j];
		auto i = j - 1;
		while (i >= 0 && teams[i].result < key.result) {
			teams[i + 1] = teams[i];
			--i;
		}
		teams[i + 1] = key;
	}
}
void print_teams(const string& filename, vector<football_team>& teams) {
	ofstream out(filename);
	if (out.is_open()) {
		for (size_t i = 0; i < teams.size(); ++i) {
			out << i + 1 << ". ";
			out << teams[i];
		}
		out.close();
	}
	else {
		cerr << "Can not open file" + filename;
	}
}
int main()
{
	string directory;
	getline(cin, directory);
	auto files = input(directory);
	auto teams = parsing(files);
	calculate_result_to_all(teams);
	sort_teams(teams);
	print_teams("result.csv", teams);
	return 0;
}