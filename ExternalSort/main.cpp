#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>


int main(int argc, char **argv)
{
	long num_entries = 0;
	int a, b, buffSize;
	std::pair<int, int> *buff = nullptr;
	std::ifstream infile("land.txt");

	infile >> num_entries;
	buffSize = (1024 / 2) / (sizeof(std::pair<int, int>))/4;

	buffSize = buffSize > num_entries ? num_entries : buffSize;

	buff = new std::pair<int, int>[buffSize];

	for (auto i = 0; i < num_entries; i++)
	{
		infile >> buff[i%buffSize].first >> buff[i%buffSize].second;
		if (i%buffSize == buffSize - 1)
		{
			std::sort(buff, buff + (i%buffSize)+1,
				[](const std::pair<int, int> &a, const std::pair<int, int> &b)->bool
			{
				if (a.first == b.first)
					return (a.second > b.second);

				return a.first < b.first;
			});
			//write buffer to file

		}
	}

	for (auto i = 0; i < num_entries; i++)
		std::cout << buff[i].first << " " << buff[i].second << std::endl;

	delete[] buff;

	return 0;
}