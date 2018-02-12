// merge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <experimental/filesystem>
#include <cassert>

namespace fs = std::experimental::filesystem;

bool inValidAddress(const std::string& line);
std::string getFolder(char address);
std::string getFileName(char address);

bool compress(const std::string& sourceRoot, char char1, char char2, std::set<std::string>& pubicKeys);
void transferData(std::set<std::string>& data, const std::string& targetRoot, char char1, char char2);

static int rawNumber = 0;
static int uniqueNumber = 0;
const std::string ALPHA_BET("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
const std::string LOG_FILE("debug.log");

int main()
{
    const std::string folderPath("./merge-from");
    if (!fs::exists(folderPath))
    {
        std::cout << "File: " << folderPath << "  does not exist..." << std::endl;
    }

    std::vector<std::string> dicts;
    for(auto& file: fs::directory_iterator(folderPath))
    {
        dicts.push_back(file.path().string());
    }

    const std::string targetRoot(""); // current

    std::cout << "Compress started" << std::endl;

    for (auto it1 = ALPHA_BET.begin(); it1 != ALPHA_BET.end(); ++it1)
    {
        for (auto it2 = ALPHA_BET.begin(); it2 != ALPHA_BET.end(); ++it2)
        {
            
            std::set<std::string> pubicKeys;
            for each (auto dict in dicts)
            {
                compress(dict, *it1, *it2, pubicKeys);
            }

            transferData(pubicKeys, targetRoot, *it1, *it2);
        }
    }

    std::ofstream logfile(LOG_FILE, std::ios_base::app);
    logfile << "Final dict records: Raw=" << rawNumber << " Unique=" << uniqueNumber << std::endl;
    
    std::cout << "Compress ended" << std::endl;
    std::cout << "Final dict records: Raw=" << rawNumber << " Unique=" << uniqueNumber << std::endl;

    system("pause.exe");
    return 0;
}

bool inValidAddress(const std::string& line)
{
    size_t s = line.length();
    return s < 30 || s > 40;
}

std::string getFolder(char folder)
{
    std::string root("merge-to/dict/");

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

std::string getFileName(char folder)
{
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

bool compress(const std::string& sourceRoot, char char1, char char2, std::set<std::string>& pubicKeys)
{
    const std::string& sourceFile = sourceRoot + getFolder(char1) + getFileName(char2);

    if (!fs::exists(sourceFile) || !fs::is_regular_file(sourceFile))
        return false;

    std::ifstream sourceInFile(sourceFile);
    if (!sourceInFile.is_open())
    {
        std::cout << "File: " << sourceFile << "does not exist..." << std::endl;
        assert(false);
        return false;
    }

    std::string line;
    while (sourceInFile >> line)
    {
        if (inValidAddress(line))
            continue;

        pubicKeys.insert(line);
        ++rawNumber;
    }

    std::ofstream logfile(LOG_FILE, std::ios_base::app);
    logfile << "Compressed source->" << sourceFile << " ->" << char1 << char2 << std::endl;
    return true;
}

void transferData(std::set<std::string>& data, const std::string& targetRoot, char char1, char char2)
{
    if (data.size() == 0)
        return;

    //create folder and file if not exist
    const std::string& firstItem = *data.begin();
    const std::string& path = getFolder(char1);
    const std::string& file = getFileName(char2);

    fs::create_directories(path);
    std::ofstream outfile(targetRoot + path + file, std::ios_base::app);

    // write data
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        outfile << *it << "\n";
    }
    uniqueNumber += data.size();
    std::ofstream logfile(LOG_FILE, std::ios_base::app);
    logfile << "Transfered target->" << targetRoot + path + file << " ->" << char1 << char2 << "                ->" << data.size() << std::endl;

    data.clear();
}

