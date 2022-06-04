// Bin2Embed.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cerrno>

bool isNumber(const std::string& str)
{
    for (char const& c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    if (argc > 0)
    {
        constexpr int bytes_per_line = 20;
        for (int i = 1; i < argc; i++)
        {
            std::filesystem::path p = argv[i];

            if (std::filesystem::exists(p))
            {
                std::stringstream output_path;
                output_path << p.parent_path().string() << "\\" << p.stem().string() << ".embed";
                std::string var_name = p.stem().string();
                std::replace(var_name.begin(), var_name.end(), ' ', '_');
                std::replace(var_name.begin(), var_name.end(), '-', '_');
                if (isNumber(var_name.substr(0, 1))) //variables cannot start with a number
                    var_name = "_" + var_name;
                std::ifstream input(p.string(), std::ios::binary);
                std::ofstream output(output_path.str(), std::ios::binary);
                std::cout << "Input file: " << p.string() << std::endl;
                std::cout << "Generating " << output_path.str() << std::endl;
                output << "namespace embedded {" << std::endl << "\tconst uint8_t " << var_name << "[] = " << std::endl << "\t{" << std::endl;
                int x = 1;
                unsigned char c;
                input.seekg(0);
                while (input.read(reinterpret_cast<char*>(&c), 1))
                {

                    if (x == 1)
                        output << "\t\t";
                    output << std::hex << "0x" << std::setw(2) << std::setfill('0') << static_cast<unsigned short>(c);
                    if (input.peek() != EOF)
                        output << ", ";

                    if (x % bytes_per_line == 0 && x > 1)
                        output << std::endl << "\t\t";
                    x++;
                }

                output << std::endl << "\t};" << std::endl << "};";
                input.close();
                output.close();
            }
            else
            {
                std::cout << "Invalid file: " << p.string() << std::endl;
                std::cin.get();
            }
        }
        std::cout << "Finished generating embed files" << std::endl;
        //std::cin.get();
    }
    else
    {
        std::cout << "Invalid argument!" << std::endl;
        std::cout << "Method 1: Bin2Embed.exe \"filepath\"" << std::endl;
        std::cout << "Method 2: Drag and drop files onto Bin2Embed.exe" << std::endl;
        std::cin.get();
    }
    
}
