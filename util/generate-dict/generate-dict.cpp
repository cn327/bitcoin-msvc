// generate-dict.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <experimental/filesystem>
#include <cassert>

namespace fs = std::experimental::filesystem;

bool inValidAddress(const std::string& line);
std::string getFolder(const std::string& address);
std::string getFileName(const std::string& address);
void transferData(std::set<std::string>& data);

static int testNumber = 0;
static int testNumber1 = 0;


int main()
{
    std::string filename("transaction-addresses-1-Out.txt");
    std::ifstream infile(filename);
    if (!infile.is_open())
    {
        std::cout << "File: " << filename << "does not exist..." << std::endl;
        return 0;
    }

    std::map<char, std::map<char, std::set<std::string>>> pubicKeys;
    std::string line;
    while (infile >> line)
    {
        if (inValidAddress(line))
            continue;

        char level1 = line.at(1);
        char level2 = line.at(2);
        std::set<std::string>& data = pubicKeys[level1][level2];
        data.insert(line);

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

    std::ofstream outfile("debug.log", std::ios_base::app);
    outfile << "File:" << filename << " Total:"  << testNumber << " Individual:" << testNumber1 << "\n";

    system("pause.exe");
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
    std::cout << "Address:" << testNumber << " " << testNumber1 << std::endl;
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

