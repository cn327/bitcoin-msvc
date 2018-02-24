// util.cpp : Defines the entry point for the console application.
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
std::string getFolder(char address);
std::string getFileName(char address);

static int decryptedNumber = 0;

int main()
{
    const std::string ALPHA_BET("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    const std::string sourceRoot("./transaction-dict-to/");
    const std::string targetRoot("./tried-key-to/");
    const std::string decryptedFile("decrypted.txt");

    if (!fs::exists(sourceRoot + "dict"))
    {
        std::cout << "File: " << sourceRoot + "dict" << "  does not exist..." << std::endl;
        return 0;
    }

    if (!fs::exists(targetRoot + "dict"))
    {
        std::cout << "File: " << targetRoot + "dict" << "  does not exist..." << std::endl;
        return 0;
    }

    std::cout << "Util started" << std::endl;
    for (auto it1 = ALPHA_BET.begin(); it1 != ALPHA_BET.end(); ++it1)
    {
        for (auto it2 = ALPHA_BET.begin(); it2 != ALPHA_BET.end(); ++it2)
        {
            const std::string& sourceFile = sourceRoot + getFolder(*it1) + getFileName(*it2);
            const std::string& targetFile = targetRoot + getFolder(*it1) + getFileName(*it2);

            std::cout << "Source->" << sourceFile << " target->" << targetFile << std::endl;

            if (!fs::exists(sourceFile) || !fs::is_regular_file(sourceFile))
                continue;
            if (!fs::exists(targetFile) || !fs::is_regular_file(targetFile))
                continue;

            std::ifstream sourceInFile(sourceFile);
            if (!sourceInFile.is_open())
            {
                std::cout << "File: " << sourceFile << "does not exist..." << std::endl;
                assert(false);
                return 0;
            }
            std::ifstream targetInFile(targetFile);
            if (!targetInFile.is_open())
            {
                std::cout << "File: " << targetFile << "does not exist..." << std::endl;
                assert(false);
                return 0;
            }


            std::set<std::string> pubicKeys;
            std::string line;
            while (sourceInFile >> line)
            {
                if (inValidAddress(line))
                    continue;

                pubicKeys.insert(line);
            }

            std::cout << "pubicKeys size-> " << pubicKeys.size() << std::endl;

            std::string decrypt;
            while (targetInFile >> decrypt)
            {
                if (inValidAddress(decrypt))
                    continue;
                //std::cout << "Finding-> " << decrypt << std::endl;
                auto it = pubicKeys.find(decrypt);
                if (it != pubicKeys.end())
                {
                    // successful
                    ++decryptedNumber;
                    std::ofstream outfile(decryptedFile, std::ios_base::app);
                    outfile << decryptedNumber << " " << decrypt << "\n";
                    outfile.flush();
                }
            }
        }
    }

    std::cout << "Util ended" << std::endl;
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

