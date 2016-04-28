#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <string>
#include <sstream>
#include <deque>

using namespace std;

//function prototypes
int split(int &buffSize, deque<string> &filenames);
//void merge(int const &buffSize, deque<string> &filenames);
void merge(int const &buffSize, deque<string> &filenames, int numFiles);
int fillBuff(ifstream &file, pair<int, int> *buff, int const &buffSize);
void writeBuff(ofstream &file, pair<int, int> *buff, int buffSize);
bool compare(const pair<int, int> &a, const pair<int, int> &b);

int main(int argc, char **argv)
{
	int buffSize;
	int numFiles;
	//pair<int, int> *buff = nullptr;
	deque<string> filenames;

	numFiles = split(buffSize, filenames);
	
	merge(buffSize, filenames, numFiles);

	return 0;
}

int split(int &buffSize, deque<string> &filenames)
{
	long num_entries = 0;
	long num_outfiles = 0;
	long file_num = 0;
	pair<int, int> *buff = nullptr;
	ifstream infile("land.txt");
	string filename;

	infile >> num_entries;
	buffSize = (1024 / 2) / (sizeof(pair<int, int>));

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

	buffSize /= 4;

	return file_num;
}

void merge(int const &buffSize, deque<string> &filenames, int numFiles)
{
	ifstream infile1, infile2;
	ofstream outfile;
	pair<int, int> *inBuff1 = nullptr, *inBuff2 = nullptr, *outBuff = nullptr;
	int i = 0, j = 0, k = 0, count1, count2, level = 0;
	string filename;

	inBuff1 = new pair<int, int>[buffSize];
	inBuff2 = new pair<int, int>[buffSize];
	outBuff = new pair<int, int>[2 * buffSize];
	
	while (numFiles)
	{
		if (numFiles == 0)
			break;
		else if (numFiles == 1)
		{
			rename(filenames[0].c_str(), "output.txt");
			filenames.pop_front();
			numFiles--;
		}
		else
		{
			int numPairs = numFiles / 2;
			level++;
			int iter;
			for (iter = 0; iter < numPairs; iter++)
			{
				filename = to_string(level).append("-").append(to_string(iter));
				outfile.open(filename.c_str());
				infile1.open(filenames[0].c_str());
				infile2.open(filenames[1].c_str());
				count1 = fillBuff(infile1, inBuff1, buffSize);
				count2 = fillBuff(infile2, inBuff2, buffSize);
				i = j = k = 0;

				while (j < count1 ||  k < count2)
				{
					if (j < count1 && k < count2)
					{
						if (compare(inBuff1[j], inBuff2[k]))
							outBuff[i++] = inBuff1[j++];
						else
							outBuff[i++] = inBuff2[k++];
					}
					else
					{
						if (j < count1)
							outBuff[i++] = inBuff1[j++];
						else if (k < count2)
							outBuff[i++] = inBuff2[k++];
					}

					if (i == 2 * buffSize)
					{
						writeBuff(outfile, outBuff, 2 * buffSize);
						i = 0;
					}

					if (j == count1 && count1!=0)
					{
						count1 = fillBuff(infile1, inBuff1, buffSize);
						j = 0;
					}

					if (k == count2 && count2!=0)
					{
						count2 = fillBuff(infile2, inBuff2, buffSize);
						k = 0;
					}

				}

				if(i)
					writeBuff(outfile, outBuff, i);

				infile1.close();
				infile2.close();
				outfile.close();
				filenames.push_back(filename);
				remove(filenames[0].c_str());
				remove(filenames[1].c_str());
				filenames.pop_front();
				filenames.pop_front();

			}

			if (numFiles == 2 * numPairs)
			{
				numFiles = numPairs;
			}
			else
			{
				numFiles = numPairs + 1;
				//iter++;
				filename = to_string(level).append("-").append(to_string(iter));
				rename(filenames[0].c_str(), filename.c_str());
				filenames.push_back(filename);
				filenames.pop_front();
			}
		}
	}
}
/*
void merge(int const &buffSize, deque<string> &filenames)
{
	//repetedly merge the files
	ifstream infile1, infile2;
	ofstream outfile("output.txt");
	pair<int, int> *inBuff1 = nullptr, *inBuff2 = nullptr, *outBuff = nullptr;
	bool file1, file2;
	int i=0, j=0, k=0, count1, count2;

	inBuff1 = new pair<int, int>[buffSize];
	inBuff2 = new pair<int, int>[buffSize];
	outBuff = new pair<int, int>[2*buffSize];

	while (!filenames.empty())
	{
		infile1.open(filenames[0]);
		filenames.pop_front();
		file1 = true;
		infile2.open(filenames[0]);
		filenames.pop_front();
		file2 = true;

		count1 = fillBuff(infile1, inBuff1, buffSize);
		count2 = fillBuff(infile2, inBuff2, buffSize);

		while (file1 || file2)
		{
			if (file1 && file2 && j < count1 && k < count2)
			{
				if(compare(inBuff1[j],inBuff2[k]))
					outBuff[i++] = inBuff1[j++];
				else
					outBuff[i++] = inBuff2[k++];
			}
			else
			{
				if(file1 && j < count1)
					outBuff[i++] = inBuff1[j++];
				else if(file2 && k < count2)
					outBuff[i++] = inBuff2[k++];
			}

			if (i == 2 * buffSize - 1)
				writeBuff(outfile,outBuff,2*buffSize);
		}
	}
}
*/
/*
int fillBuff(ifstream &file, pair<int, int> *buff, int const &buffSize)
{
	int count = 0;
	while (count < buffSize && ~file.eofbit)
	{
		file >> buff[count].first >> buff[count].second;
		count++;
	}
	return count;
}
*/

int fillBuff(ifstream &file, pair<int, int> *buff, int const &buffSize)
{
	int count = 0;
	string temp;
	while (count < buffSize && getline(file,temp))
	{
		stringstream tempstream(temp);
		tempstream >> buff[count].first >> buff[count].second;
		count++;
	}
	return count;
}

void writeBuff(ofstream &file, pair<int, int> *buff, int buffSize)
{
	for (int i = 0; i < buffSize; i++)
	{
		file << buff[i].first << " " << buff[i].second << endl;
	}
}

bool compare(const pair<int, int> &a, const pair<int, int> &b)
{
	if (a.first == b.first)
		return (a.second > b.second);

	return a.first < b.first;
}