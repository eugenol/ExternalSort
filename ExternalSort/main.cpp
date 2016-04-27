#include <iostream>
#include <fstream>
#include <utility>


int main(int argc, char **argv)
{
	long num_entries = 0;
	int a, b, buffSize;
	std::pair<int, int> *buff = nullptr;
	std::ifstream infile("land.txt");

	infile >> num_entries;
	buffSize = (1024 / 2) / (sizeof(std::pair<int, int>))/4;

	buff = new std::pair<int, int>[buffSize];

	for (auto i = 0; i < num_entries; i++)
	{
		infile >> buff[i%buffSize].first >> buff[i%buffSize].second;
		if (i%buffSize == buffSize - 1)
		{
			//sort buffer
			//write buffer to file

		}
	}

	for (auto i = 0; i < num_entries; i++)
		std::cout << a << " " << b << std::endl;

	return 0;
}