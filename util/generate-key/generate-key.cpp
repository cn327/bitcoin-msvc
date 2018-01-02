// generate-key.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <experimental/filesystem>
#include <cassert>
#include <sstream>

namespace fs = std::experimental::filesystem;

bool inValidAddress(const std::string& line);
std::string getFolder(const std::string& address);
std::string getFileName(const std::string& address);
void transferData(std::set<std::string>& data);

static int testNumber = 0;
static int testNumber1 = 0;


int main()
{
	std::string filename("Tried-20180102-laptop.txt");
	std::ifstream infile(filename);
	if (!infile.is_open())
	{
		std::cout << "File: " << filename << "does not exist..." << std::endl;
		return 0;
	}

	std::map<char, std::map<char, std::set<std::string>>> pubicKeys;
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
        std::string target;
        std::string dummy;
        iss >> dummy; iss >> dummy; iss >> dummy; iss >> dummy;
        iss >> target;
        iss >> dummy;

		if (inValidAddress(target))
			continue;

		char level1 = target.at(1);
		char level2 = target.at(2);
		std::set<std::string>& data = pubicKeys[level1][level2];
		data.insert(target);

		if (data.size() >= 1000) // in case of memory overhead
		{
			transferData(data);
		}

		// for test small number
		++testNumber;
#if 0
		if (testNumber >= 1000000)
			break;
#endif
	}

	for (auto it1 = pubicKeys.begin(); it1 != pubicKeys.end(); ++it1)
	{
		auto& value1 = it1->second;
		for (auto it2 = value1.begin(); it2 != value1.end(); ++it2)
		{
			transferData(it2->second);
		}
	}

	return 0;
}

void transferData(std::set<std::string>& data)
{
	if (data.size() == 0)
		return;

	//create folder and file if not exist
	const std::string& firstItem = *data.begin();
	const std::string& path = getFolder(firstItem);
	const std::string& file = getFileName(firstItem);

	fs::create_directories(path);
	std::ofstream outfile(path + file, std::ios_base::app);

	// write data
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		outfile << *it << "\n";
		// for test small number
		++testNumber1;
	}

	// clear
	data.clear();
}

bool inValidAddress(const std::string& line)
{
	size_t s = line.length();
	return s < 30 || s > 40;
}

std::string getFolder(const std::string& address)
{
	char folder = address.at(1);
	std::string root("dict/");

	if (isdigit(folder))
		root = root + "D_" + folder + "/";
	else if (isupper(folder))
		root = root + "U_" + folder + "/";
	else if (islower(folder))
		root = root + "L_" + folder + "/";
	else
		assert(false);

	return root;
}

std::string getFileName(const std::string& address)
{
	char folder = address.at(2);
	std::string file;

	if (isdigit(folder))
		file = file + "D_" + folder;
	else if (isupper(folder))
		file = file + "U_" + folder;
	else if (islower(folder))
		file = file + "L_" + folder;
	else
	{
		file = "error";
		assert(false);
	}
	return file;
}

