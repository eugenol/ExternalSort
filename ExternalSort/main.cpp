#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//function prototypes
void split(int &buffSize, vector<string> &filenames);

int main(int argc, char **argv)
{
	int buffSize;
	//pair<int, int> *buff = nullptr;
	vector<string> filenames;

	split(buffSize, filenames);
	
	return 0;
}

void split(int &buffSize, vector<string> &filenames)
{
	long num_entries = 0;
	long num_outfiles = 0;
	long file_num = 1;
	pair<int, int> *buff = nullptr;
	ifstream infile("land.txt");
	string filename;

	infile >> num_entries;
	buffSize = (1024 / 2) / (sizeof(pair<int, int>)) / 4;

	buffSize = buffSize > num_entries ? num_entries : buffSize;

	buff = new pair<int, int>[buffSize];

	num_outfiles = (num_entries / buffSize) + (num_entries%buffSize == 0 ? 0 : 1);

	for (auto i = 0; i < num_entries; i++)
	{
		infile >> buff[i%buffSize].first >> buff[i%buffSize].second;
		if (i%buffSize == buffSize - 1 || i == num_entries-1)
		{
			sort(buff, buff + (i%buffSize)+1,
				[](const pair<int, int> &a, const pair<int, int> &b)->bool
			{
				if (a.first == b.first)
					return (a.second > b.second);

				return a.first < b.first;
			});
			//filename = static_cast<ostringstream*>(&(ostringstream() << file_num))->str();
			filename = to_string(file_num);
			ofstream outfile(filename);
			for (auto j = 0; j <= i%buffSize; j++)
				outfile << buff[j].first << " " << buff[j].second << endl;
			filenames.push_back(filename);
			file_num++;
		}
	}

	delete[] buff;
}