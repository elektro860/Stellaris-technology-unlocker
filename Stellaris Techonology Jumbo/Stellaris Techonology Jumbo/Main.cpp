// Stellaris Techonology Jumbo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <Windows.h>
#include <iostream>
#include "Parser.hpp"


const auto _debug_target = "D:/SteamLibrary/steamapps/common/Stellaris/common/technology";
int main()
{
    std::string relativePath = filesystem::current_path().string();
    Start:
    std::cout << "Select your target path: ( Stellaris/common/technology ) for example\n";
    std::string targetPath = "";
    std::cin >> targetPath;

    if (targetPath == "debug")
        targetPath = _debug_target;

    if (!filesystem::exists(targetPath))
    {
        system("cls");
        std::cout << targetPath + " is not a valid path\n\n";
        goto Start;
    }

    auto start = std::chrono::high_resolution_clock::now();

    directory_Viewer targetDirectory = directory_Viewer(targetPath);

    targetDirectory.Update_Content((uint32_t)targetDirectory.files.size());
    
    
    std::fstream outputFile("out.txt", std::ofstream::out | std::ofstream::trunc);
    
#if 0
    for (directory_file *file : targetDirectory.files) 
    {
        if (file->content != nullptr)
        {
            outputFile << "___________________________________________________________\n";
            outputFile << file->fileName + ":\n";
            std::vector<std::string> technologies = getTechnologiesFromFile(file);
            for (std::string& technology : technologies)
            {
                outputFile << " Technology - " + technology + "\n";
            }
        }
    }*/
#else
    for (directory_file* file : targetDirectory.files)
    {
        if (file->content != nullptr)
        {
            std::vector<std::string> technologies = getTechnologiesFromFile(file);
            for (std::string& technology : technologies)
            {
                outputFile << " give_techonology={which=" + technology + "}\n"; // From a modder, for modders
            }
        }
    }
#endif
    outputFile.close();

    //std::cout << "Output: " + relativePath + "out.txt\n";
    std::cout << "Elapsed time - " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count())+"ms";
    std::cout << "\nA folder with a name out.txt was generated, all of the technologies should be there.";
    std::cout << "\nClosing in 20 seconds";
    Sleep(20000); // Closes after 3 seconds
    return 0;
}
